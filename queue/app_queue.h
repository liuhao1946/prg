#ifndef _APP_QUEUE_
#define _APP_QUEUE_

#include "stdint.h"

#define APP_QUEUE_VERSION "2.0.0"

typedef enum
{
	Q_SUCCESS,
	Q_ERROR,
	Q_NONE,
	Q_FULL,
}Q_status_t;

typedef uint16_t q_size_t;

//qcb: queue control block
typedef struct
{
	q_size_t head;
	q_size_t end;
	q_size_t q_len;
	q_size_t size;
	q_size_t overflow_ctr;
}qcb_t;

typedef enum
{
	BLOCK,
	NO_BLOCK,
}q_overflow_t;

/**
 * @brief Initializing the circular queue(FIFO).
 * 
 * Note: This is a **generic queue** that does not require dynamic memory allocation.
 *
 * @param[in] handle Queue Handle.
 * 
 * @param[in] q_max  Queue buffer size.
 * 
 * @param[in] node_size Size of an element in the queue.
 * 
 * @param[in] overflow_ctr Blocking or non-blocking when the queue is full.
 *
 * BLOCK - Blocking when the queue is full.
 * 
 * NO_BLOCK - No blocking when the queue is full.
 * 
 * @return Operation success returns Q_SUCCESS.
 * 
 * Example usage:
 * 
 * #define Q_BUF_LEN 	10
 * 
 * typedef {
 * 	uint8_t s1;
 *  uint16_t s2;
 * }ele_t;
 * 
 * typedef {
 * //Note: All queues must be defined qcb and must be placed in the first position of the structure
 * 	qcb_t qcb;  
 *  ele_t buf[Q_BUF_LEN];
 * }q_t;
 * 
 * q_t q;
 * 
 * app_queue_init(&q, Q_BUF_LEN, sizeof(ele_t), NO_BLOCK);
 * 
 * Note: The **actual elements** that can be stored in the queue are **(Q_BUF_LEN-1)**.
 */
void app_queue_init(void *handle, q_size_t q_max,q_size_t element_size, q_overflow_t overflow_ctr);

/**
 * @brief Data entry queue.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @param[in] element  An element in the queue.
 * 
 * @return Operation success returns Q_SUCCESS.
 * 
 * Example usage:
 * 
 * ele_t ele;
 * 
 * ele.s1 = 60;
 * ele.s2 = 6000;
 * app_enqueue(&q, &ele);
 * 
 * Note: To initialize the queue first
 */
Q_status_t app_enqueue(void *handle, void *element);

/**
 * @brief Data out of queue
 * 
 * @param[in] handle Queue Handle.
 * 
 * @param[out] element  An element in the queue.
 * 
 * @return Operation success returns Q_SUCCESS.
 * 
 * Example usage:
 * 
 * ele_t ele;
 * 
 * app_dequeue(&q, &ele);
 * 
 * Note: To initialize the queue first
 */
Q_status_t app_dequeue(void *handle, void *element);

/**
 * @brief Determine if the queue is empty.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @return Return Q_NONE means the queue is empty.
 * 
 * Example usage:
 * 
 * if (app_queue_is_none() == Q_NONE) {
 * 		
 * }
 */
Q_status_t app_queue_is_none(void *handle);

/**
 * @brief Search for the specified element in the queue.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @param[in] element  Elements to search.
 * 
 * @return Returns Q_NONE to indicate that there is no specified element in the queue
 * 
 * Example usage:
 * 
 * ele_t ele; 
 * 
 * ele.s1 = 0x01;
 * ele.s2 = 0x02;
 * //Check if ele is in the queue
 * if (app_queue_search(&q, &ele) == Q_NONE) {
 * 		
 * }
 */
Q_status_t app_queue_search(void *handle, void *element);

/**
 * @brief Clear Queue
 * 
 * @param[in] handle Queue Handle.
 * 
 * @return None
 */
void app_queue_clr(void *handle);

/**
 * @brief Get the number of elements in the queue.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @return Returns number of elements.
 *
 */
q_size_t app_queue_get_ele_num(void *handle);

/**
 * @brief Get the number of elements that can still be stored in the queue.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @return Returns Remain number of elements.
 *
 */
q_size_t app_queue_get_remain_ele_num(void *handle);

/**
 * @brief traversal through all the elements in the queue.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @param[in] idx Indexing of queue elements.
 * 
 * Note:idx=0 indicates the location pointed to by the queue head pointer.
 *		idx=1 indicates the next position pointed to by the queue head pointer.
 *		...
 *      idx=app_queue_get_ele_num()-1 Indicates the last element of the queue
 * 
 * @param[out] element Returns the element in the queue corresponding to the index.
 * 
 * @return Returns Q_NONE to indicate that there is no specified element in the queue
 * 
 * Example usage:
 * 
 * ele_t ele; 
 * 
 * for (uint16_t i = 0; i < app_queue_get_ele_num(); i++) {
 * 		app_queue_traversal(&q, &ele i);
 * 		printf("%d, %d\n", ele.s1, ele.s2);
 * }
 */
Q_status_t app_queue_traversal(void *handle, q_size_t idx, void *element);

/**
 * @brief Modify the element corresponding to the specified index in the queue.It needs to be used with app_queue_traversal().
 * 
 * @param[in] handle Queue Handle.
 * 
 * @param[in] idx @ref app_queue_traversal().
 * 
 * @param[in] new_ele Replace the specified element with new_ele.
 * 
 * @return Returns Q_SUSCESS to indicate successful replacement.
 * 
 * Example usage:
 * 
 * ele_t ele,new_ele; 
 * 
 * for (uint16_t i = 0; i < app_queue_get_ele_num(); i++) {
 * 		app_queue_traversal(&q, &ele i);
 * 		if (ele.s1 == 0x01) {
 * 			new_ele.s1 = 0x02;
 * 			app_queue_modify_speci_element(&q, i, &new_ele);
 * 		}
 * 		printf("%d, %d\n", ele.s1, ele.s2);
 * }
 */
Q_status_t app_queue_modify_speci_element(void *handle, q_size_t idx, void *new_ele);


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
typedef struct
{
	void *data;
	uint16_t head;
	uint16_t end;
	uint16_t buf_size;
	uint16_t buf_pos;
	uint8_t size;
}batch_q_t;

/**
 * @brief FIFO ring queue initialization.Mainly used for bulk storage of the same type of data, such as log.
 * 
 * @param[in] bq Queue Handle.
 * 
 * @param[in] p  Data buffer pointer.
 * 
 * @param[in] len Buffer length.
 * 
 * Note: Buffer length must be a multiple of 2.
 * 
 * @param[in] size Number of bytes for an element in the queue.
 *
 * 1 - uint8_t
 * 
 * 2 - uint16_t 
 * 
 * 4 - uint32_t 
 * 
 * @return None.
 * 
 * Example usage:
 * 
 * #define BQ_BUF_LEN		1024
 * 
 * uint8_t buf[1024];
 * 
 * batch_q_t bq;
 * 
 * app_batch_queue_init(&bq, buf, BQ_BUF_LEN, 1);
 */
void app_batch_queue_init(batch_q_t *bq, void *p, uint16_t len, uint8_t size);

/**
 * @brief Data entry queue.
 * 
 * @param[in] bq Queue Handle.
 * 
 * @param[in] p  Data buffer pointer
 * 
 * @param[in] len Length of data to be written to the queue
 * 
 * @return Returns the length of the data written to the queue
 * 
 * Note: If the data to enter the queue exceeds the queue length, the earlier data in the queue will be overwritten
 * 
 * Example usage:
 * 
 * app_batch_data_enqueue(&bp, buf, 100);
 */
int32_t app_batch_data_enqueue(batch_q_t *bq, void *p, uint16_t len);

/**
 * @brief Data out of queue.
 * 
 * @param[in] bq Queue Handle.
 * 
 * @param[in] dequeue_len Length of data to be queued out.
 * 
 * @param[out] buf Out Queue Data Buffer.
 * 
 * @param[in] buf_max_len Out Queue Data Buffer Length.
 * 
 * Note: 
 * 1. If dequeue_len > buf_max_len, fetch only buf_max_len data in the queue.
 * 
 * 2. If dequeue_len <= buf_max_len, fetch only dequeue_len data from the queue.
 * 
 * @return Returns the length of the data that has been removed from the queue.
 * 
 * Example usage:
 * 
 * //Fetch 256 data from the queue
 * app_batch_data_dequeue(&bp, 256, buf, 256);
 */
int32_t app_batch_data_dequeue(batch_q_t *bq, uint16_t dequeue_len, void *buf,uint16_t buf_max_len);

/**
 * @brief Get the number of elements that can still be stored in the queue.
 * 
 * @param[in] bq Queue Handle.
 * 
 * @return Returns Remain number of elements.
 *
 */
uint16_t app_batch_queue_remain_ele_num(batch_q_t *bq);

/**
 * @brief Get the number of elements in the queue.
 * 
 * @param[in] handle Queue Handle.
 * 
 * @return Returns number of elements.
 *
 */
uint16_t app_batch_queue_get_ele_num(batch_q_t *bq);

/**
 * @brief Clear Queue
 * 
 * @param[in] bq Queue Handle.
 * 
 * @return None
 */
void app_batch_queue_clr(batch_q_t *bq);

/**
 * @brief Determine if the queue is empty.
 * 
 * @param[in] bq Queue Handle.
 * 
 * @return Return Q_NONE means the queue is empty.
 * 
 * Example usage:
 * 
 * if (app_batch_queue_is_none() == Q_NONE) {
 * 		
 * }
 */
Q_status_t app_batch_queue_is_none(batch_q_t *bq);

#endif










