/*
 * @Author: bds
 * @Date: 2023-02-11
 * @LastEditors: liuhao1946 2401569420@qq.com
 * @LastEditTime: 2023-02-11 18:42:02
 * @FilePath:
 * @Description: FIFO queue module
 */

#include "app_queue.h"
#include "string.h"

#define SKIP_QCB_SIZE	(sizeof(qcb_t)/sizeof(q_size_t))

void app_queue_init(void* handle, q_size_t q_max, q_size_t element_size, q_overflow_t overflow_ctr)
{
	qcb_t p;

	q_size_t *pq = (q_size_t*)handle;
	uint16_t len = sizeof(qcb_t);

	memcpy(&p, pq, len);
	p.q_len = q_max * element_size;
	p.size = element_size;
	p.overflow_ctr = overflow_ctr;
	memcpy(pq, &p, len);
}

void app_queue_clr(void* p_q1)
{
	qcb_t p;
	uint16_t len = sizeof(qcb_t);

	memcpy(&p, (q_size_t*)p_q1, len);
	p.end = 0;
	p.head = 0;
	memcpy((q_size_t*)p_q1, &p, len);
}

Q_status_t app_queue_is_none(void* handle)
{
	qcb_t p;

	memcpy(&p, (q_size_t*)handle, sizeof(qcb_t));

	if (p.head == p.end) {
		return Q_NONE;
	}

	return Q_SUCCESS;
}

Q_status_t app_enqueue(void* handle, void* element)
{
	qcb_t p;
	uint8_t len = sizeof(qcb_t);
	q_size_t *q = (q_size_t*)handle;
	uint8_t* qsrc = (uint8_t*)element;

	memcpy(&p, q, len);
	if (p.head == (p.end + p.size) % p.q_len) {
		if (p.overflow_ctr == BLOCK) {
			return Q_FULL;
		} else {
			p.head = (p.head + p.size) % p.q_len;
		}
	}
	memcpy((uint8_t*)(q + SKIP_QCB_SIZE) + p.end, qsrc, p.size);
	p.end = (p.end + p.size) % p.q_len;
	memcpy(q, &p, len);

	return Q_SUCCESS;
}

Q_status_t app_dequeue(void* handle, void* element)
{
	qcb_t p;
	uint8_t len = sizeof(qcb_t);
	q_size_t *q = (q_size_t*)handle;
	uint8_t *qdes = (uint8_t*)element;

	memcpy(&p, q, len);

	if (p.head == p.end) {
		return Q_NONE;
	}

	memcpy(qdes, (uint8_t*)(q + SKIP_QCB_SIZE) + p.head, p.size);
	p.head = (p.head + p.size) % p.q_len;
	memcpy(q, &p, len);

	return Q_SUCCESS;
}

Q_status_t app_queue_search(void* handle, void* element)
{
	qcb_t p;
	uint8_t* ps, *pt, *pm;
	uint8_t i;

	q_size_t *q = (q_size_t*)handle;
	uint8_t* qsrc = (uint8_t*)element;

	memcpy(&p, q, sizeof(qcb_t));

	ps = (uint8_t*)(q + SKIP_QCB_SIZE);
	while (p.head != p.end) {
		pt = ps + p.head;
		pm = qsrc;

		for (i = 0; i < p.size; i++) {
			if (*pt++ != *pm++) break;
		}
		if (i == p.size) {
			return Q_SUCCESS;
		} else {
			p.head += p.size;
			if (p.head >= p.q_len) {
				p.head = 0;
			}
		}
	}

	return Q_NONE;
}

q_size_t app_queue_get_ele_num(void* queue)
{
	qcb_t p;
	q_size_t len;

	q_size_t *q = (q_size_t*)queue;
	memcpy(&p, q, sizeof(qcb_t));

	if (p.end >= p.head) {
		len = (p.end - p.head) / p.size;
	} else {
		len = (p.q_len - p.head + p.end) / p.size;
	}

	return len;
}

q_size_t app_queue_get_remain_ele_num(void* queue)
{
	qcb_t p;

	q_size_t *q = (q_size_t*)queue;
	memcpy(&p, q, sizeof(qcb_t));

	q_size_t len = p.q_len / p.size - app_queue_get_ele_num(queue) - 1;

	return len;
}

Q_status_t app_queue_traversal(void* handle, q_size_t idx, void* element)
{
	qcb_t qcb;
	q_size_t head;

	q_size_t len = app_queue_get_ele_num(handle);
	if (idx >= len) {
		return Q_ERROR;
	}
	q_size_t *q = (q_size_t*)handle;
	memcpy(&qcb, q, sizeof(qcb_t));
	head = (qcb.head + idx * qcb.size) % qcb.q_len;
	memcpy(element, (uint8_t*)(q + SKIP_QCB_SIZE) + head, qcb.size);

	return Q_SUCCESS;
}

Q_status_t app_queue_modify_speci_element(void* handle, q_size_t idx, void* p_replace)
{
	qcb_t qcb;
	q_size_t head;

	q_size_t len = app_queue_get_ele_num(handle);
	if (idx >= len) {
		return Q_ERROR;
	}
	q_size_t *q = (q_size_t*)handle;
	memcpy(&qcb, q, sizeof(qcb_t));
	head = (qcb.head + idx * qcb.size) % qcb.q_len;
	memcpy((uint8_t*)(q + SKIP_QCB_SIZE) + head, p_replace, qcb.size);

	return Q_SUCCESS;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
uint16_t app_batch_queue_remain_ele_num(batch_q_t *bq)
{
	uint16_t len;

	if (bq->end >= bq->head) {
		len = bq->buf_size - bq->end + bq->head - 1;
	} else {
		len = bq->head - (bq->end + 1);
	}

	return len;
}

uint16_t app_batch_queue_get_ele_num(batch_q_t *bq)
{
	uint16_t len;

	len = bq->buf_size - app_batch_queue_remain_ele_num(bq) - 1;

	return len;
}

int32_t app_batch_data_enqueue(batch_q_t *bq, void *p, uint16_t len)
{
	uint16_t len_tp;
	uint16_t head, end;
	uint16_t temp_end;
	uint16_t offset = 0;

	if (p == 0 || len == 0 || len > bq->buf_size) {
		return 0;
	}

	head = bq->head;
	end = bq->end;

	temp_end = (end + len) & bq->buf_pos;
	if (end < head && temp_end >= head) {
		offset = temp_end - head + 1;
	} else if (end >= head) {
		if (end + len >= bq->buf_size && temp_end >= head) {
			offset = temp_end - head + 1;
		}
	}

	len_tp = len;
	if (bq->size == 1) {
		uint8_t *bq8, *q8;

		bq8 = (uint8_t *)bq->data;
		q8 = (uint8_t *)p;

		while (len_tp--) {
			bq8[end++ & bq->buf_pos] = *q8++;
		}
	} else if (bq->size == 2) {
		uint16_t *bq16, *q16;

		bq16 = (uint16_t *)bq->data;
		q16 = (uint16_t *)p;
		while (len_tp--) {
			bq16[end++ & bq->buf_pos] = *q16++;
		}
	} else if (bq->size == 4) {
		uint32_t *bq32, *q32;

		bq32 = (uint32_t *)bq->data;
		q32 = (uint32_t *)p;
		while (len_tp--) {
			bq32[end++ & bq->buf_pos] = *q32++;
		}
	}

	bq->head = (bq->head + offset) & bq->buf_pos;
	bq->end = (bq->end + len) & bq->buf_pos;

	return len;
}

int32_t app_batch_data_dequeue(batch_q_t *bq, uint16_t dequeue_len, void *buf,uint16_t buf_max_len)
{
	uint16_t len_tp;
	uint16_t end, head;

	end = bq->end;
	head = bq->head;

	if (end == head) {
		return -1;
	} else if (head < end) {
		if (head + dequeue_len > end) {
			dequeue_len = end - head;
		}
	} else {
		len_tp = bq->buf_size - head;
		if (dequeue_len > len_tp + end)
		{
			dequeue_len = bq->buf_size - head + end;
		}
	}

	if (dequeue_len >= buf_max_len) {
		dequeue_len = buf_max_len;
	}
	len_tp = dequeue_len;
	if (bq->size == 1) {
		uint8_t *bq8, *q8;

		bq8 = (uint8_t *)bq->data;
		q8 = (uint8_t *)buf;
		while (len_tp--)
		{
			*q8++ = bq8[head++ & bq->buf_pos];
		}
	} else if (bq->size == 2) {
		uint16_t *bq16, *q16;
		bq16 = (uint16_t *)bq->data;
		q16 = (uint16_t *)buf;
		while (len_tp--)
		{
			*q16++ = bq16[head++ & bq->buf_pos];
		}
	}
	else if (bq->size == 4) {
		uint32_t *bq32, *q32;
		bq32 = (uint32_t *)bq->data;
		q32 = (uint32_t *)buf;
		while (len_tp--)
		{
			*q32++ = bq32[head++ & bq->buf_pos];
		}
	}

	bq->head = (bq->head + dequeue_len) & bq->buf_pos;

	return dequeue_len;
}

Q_status_t app_batch_queue_is_none(batch_q_t *bq)
{
	if (bq->end == bq->head) {
		return Q_NONE;
	}

	return Q_SUCCESS;
}

void app_batch_queue_clr(batch_q_t *bq)
{
	bq->end = 0;
	bq->head = 0;
}

void app_batch_queue_init(batch_q_t *bq, void *p, uint16_t len, uint8_t size)
{
	bq->head = 0;
	bq->end = 0;
	bq->size = size;
	bq->buf_size = len;
	bq->buf_pos = len - 1;
	bq->data = p;
}
