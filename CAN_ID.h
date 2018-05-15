#ifndef _CAN_ID_H_
#define _CAN_ID_H_

#define CAN_PEDALS_ID       0
#define CAN_FRONTAL_ID      1
#define CAN_RETRO_ID        2

#if defined(_FRONTAL_)
#define NODE_ID				1
#elif defined(_RETRO_)
#define NODE_ID				2
#endif

#define VCU_NODE_ID			3

#endif /* _CAN_ID_H_ */