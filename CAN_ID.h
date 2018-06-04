#ifndef _CAN_ID_H_
#define _CAN_ID_H_

#define VCU_NODE_ID         2

#if defined(_FRONTAL_)
#define NODE_ID				1
#elif defined(_RETRO_)
#define SCU_FRONTAL_ID      1
#define NODE_ID				3
#endif

#endif /* _CAN_ID_H_ */