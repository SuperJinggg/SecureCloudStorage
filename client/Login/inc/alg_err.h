//#ifndef ALG_ERR_H
//#define ALG_ERR_H

#ifdef unix
#define OMP_SUCCESS						0
#define OMP_NULL_PTR_ERR				1
#define OMP_HASH_TYPE_NOT_SUPPORT		2
#define OMP_HMAC_ERR					3
#define OMP_MEMORY_NOT_ENOUGH_ERR		4
#define OMP_IMSI_INVALID_ERR			5
#define OMP_DES_KEY_LEN_ERR				6
#define OMP_PARAM_LEN_ERR				7
#define OMP_HEX_STR_DATA_ERR			8
#define OMP_TIME_STAMP_NOT_EQUALS_ERR	9
#define OMP_INIT_RAND_FAILED_ERR		10
#define OMP_GET_PID_ERR					11
#else
#define OMP_SUCCESS						0
#define OMP_NULL_PTR_ERR				1
#define OMP_HASH_TYPE_NOT_SUPPORT		2
#define OMP_HMAC_ERR					3
#define OMP_MEMORY_NOT_ENOUGH_ERR		4
#define OMP_IMSI_INVALID_ERR			5
#define OMP_DES_KEY_LEN_ERR				6
#define OMP_PARAM_LEN_ERR				7
#define OMP_HEX_STR_DATA_ERR			8
#define OMP_TIME_STAMP_NOT_EQUALS_ERR	9
#define OMP_INIT_RAND_FAILED_ERR		10
#define OMP_GET_PID_ERR					11
#endif
