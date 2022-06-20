#ifndef _COM_PARAM_H
#define _COM_PARAM_H

// ローカルソケット用の擬似ファイル名
#define SOCK_FILENAME "/tmp/_chat_.tmp"
// 接続失敗時にこの秒数待ってからリトライする
#define RETRY_INTERVAL 1
// 送受信バッファサイズ(行頭ラベル込みでこのバイト数が上限)
#define LINE_LEN_MAX 1024
// 同時接続可能なクライアント数上限
#define CLIENT_MAX 5

#endif
