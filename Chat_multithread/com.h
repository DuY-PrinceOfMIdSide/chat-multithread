#ifndef _COM_H
#define _COM_H
#include "com_param.h"

// 無効なクライアントID(connect失敗時に返す値)
#define INVALID_CLIENT_ID (-1)

// ***** サーバ用関数
// 初期化。最初に一回呼び出す
void com_server_init(void);
// クライアントからの要求を待って接続
// クライアントIDを返す(MT-unsafeなので複数スレッドから同時に呼ばないこと)
int com_server_connect(void);
// 指定IDの接続を切断
void com_server_disconnect(int id);
// 指定IDのクライアントが接続中なら真を返す
int com_is_client_connected(int id);

// ***** クライアント用関数
// 初期化。最初に一回呼び出す
void com_client_init(void);
// サーバに接続要求し接続
void com_client_connect(void);
// 現在の接続を切断
void com_client_disconnect(void);

// 共通関数
// msgが指す文字列を送信
int com_send(int id, char *msg);
// msgが指す位置に受信文字列を格納
int com_recv(int id, char *msg);

#endif
