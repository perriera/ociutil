#ifndef _OCIHANDLE_H
#define _OCIHANDLE_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	OCIEnv     *phEnv;
	OCISvcCtx  *phService;
	OCIError   *phErr;
	OCIStmt    *phStmt;
	OCIServer  *phServer;
	OCISession *phSession;
} OCIHANDLE;

#ifdef __cplusplus
}
#endif

#endif
