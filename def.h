/**
 *  @file           def.h
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          CANopen DS301 definitions
 */

#ifndef _DEF_H_
#define _DEF_H_

/**
 *  @addtogroup CAN_network_module
 *   @{
 */

/**
 *  @defgroup CANopen_Func_Codes CANopen Function Codes
 *   @{
 *   CANopen function codes defined in DS301 profile.
 */

/**
 *  @def NMT
 *  @brief NMT function code
 */
#define NMT	       0x0

/**
 *  @def SYNC
 *  @brief SYNC function code
 */
#define SYNC       0x1

/**
 *  @def TIME_STAMP
 *  @brief TIME_STAMP function code
 */
#define TIME_STAMP 0x2

/**
 *  @def PDO1tx
 *  @brief PDO1tx function code
 */
#define PDO1tx     0x3

/**
 *  @def PDO1rx
 *  @brief PDO1rx function code
 */
#define PDO1rx     0x4

/**
 *  @def PDO2tx
 *  @brief PDO2tx function code
 */
#define PDO2tx     0x5

/**
 *  @def PDO2rx
 *  @brief PDO2rx function code
 */
#define PDO2rx     0x6

/**
 *  @def PDO3tx
 *  @brief PDO3tx function code
 */
#define PDO3tx     0x7

/**
 *  @def PDO3rx
 *  @brief PDO3rx function code
 */
#define PDO3rx     0x8

/**
 *  @def PDO4tx
 *  @brief PDO4tx function code
 */
#define PDO4tx     0x9

/**
 *  @def PDO4rx
 *  @brief PDO4rx function code
 */
#define PDO4rx     0xA

/**
 *  @def SDOtx
 *  @brief SDOtx function code
 */
#define SDOtx      0xB

/**
 *  @def SDOrx
 *  @brief SDOrx function code
 */
#define SDOrx      0xC

/**
 *  @def NODE_GUARD
 *  @brief NODE GUARD function code
 */
#define NODE_GUARD 0xE

/**
 *  @def LSS
 *  @brief LSS function code
 */
#define LSS 	   0xF

/**
 *  @def GET_FUNC_CODE(COB_ID) 
 *  @brief Extract function code from COB-ID
 */
#define GET_FUNC_CODE(COB_ID)	(COB_ID >> 7)

/**
 *  @def SET_FUNC_CODE(COB_ID) 
 *  @brief Set function code to COB-ID
 */
#define SET_FUNC_CODE(COB_ID)   (COB_ID << 7)

/**
 *  @}
 */

/**
 *  @addtogroup CANopen_NMT_group
 *   @{
 */

/**
 *  @defgroup CANopen_NMT_speficications CANopen NMT Command Specifiers
 *   @{
 */

/**
 *  @def NMT_Start_Node
 *  @brief 'go Operational' command specifier
 */
#define NMT_Start_Node              0x01

/**
 *  @def NMT_Stop_Node
 *  @brief 'stop Node' command specifier
 */
#define NMT_Stop_Node               0x02

/**
 *  @def NMT_Enter_PreOperational
 *  @brief 'go PreOperational' command specifier
 */
#define NMT_Enter_PreOperational    0x80

/**
 *  @def NMT_Reset_Node
 *  @brief 'reset Node' command specifier
 */
#define NMT_Reset_Node              0x81

/**
 *  @def NMT_Reset_Comunication
 *  @brief 'reset Communication' command specifier
 */
#define NMT_Reset_Comunication      0x82

/**
 *  @}
 */

/**
 *  @}
 */

/**
 *  @}
 */

#endif /* _DEF_H_ */