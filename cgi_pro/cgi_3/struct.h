#define FARM_NUM 		5
//#define LEN 6
#define LEN 12

#define QUEUE_MSG_LEN  1
typedef struct {
	unsigned char farm_status;
	unsigned char led_status;
	unsigned char buzzer_status;
	unsigned char fan_status;//风扇
	unsigned char bump_status;//水泵
//	unsigned char digitrom_status;//数码管
	unsigned char GPRS_status;
//	unsigned char relay_status;//继电器
	unsigned char camera_status;
	char samplingtime[20];
	float temperature;
	float temperature_MAX;
	float temperature_MIN;
	float humidity;
	float humidity_MAX;
	float humidity_MIN;
	float illumination;
	float illumination_MAX;
	float illumination_MIN;
	float adc_voltage;
	int farm_no;
} farm_info;

struct control_parameter {
	int farm_num;
	float temperature_MAX;
	float temperature_MIN;
	float humidity_MAX;
	float humidity_MIN;
	float illumination_MAX;
	float illumination_MIN;
};

union text {
	unsigned char led;
	unsigned char buzzer;
	unsigned char camera;
	unsigned char fan;
	unsigned char relay;
	unsigned char uart;
	char phone[24];
	struct control_parameter parameter;
};

struct message {
	long type;
	long msg_type;
	union text text;
};

struct sqlite_operation {
	int table_select_mask;
	int env_operation_mask;
	int table_operation_mask;
};

struct env_info_clien_addr
{
	farm_info farm_no[FARM_NUM];
};

typedef struct _snode_
{
	struct sqlite_operation cmd; 	//对数据库进行操作的命令字
	struct env_info_clien_addr env_info_clien_addr_t;
	int farm_num; 					//农场的编号
	struct _snode_ *next;
} slinknode, *slinklist;

#if 0
//typedef void* uart_cache_data;

typedef struct _uart_node_ {
	unsigned char data;
	struct _uart_node_ *next;
}uart_cache_node, *uart_cache_list;
#endif

typedef void* uart_cache_data;

typedef struct _uart_node_ {
	uart_cache_data data;
	struct _uart_node_ *next;
}uart_cache_node, *uart_cache_list;

//
typedef struct msg_pack
{
	unsigned char text[LEN];
}link_datatype;

typedef struct _node_
{
	link_datatype data;
	struct _node_ *next;
}linknode, *linklist;
