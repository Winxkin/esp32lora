#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lora.h"
#include "uart.h"
#include "delay.h"

uint8_t buf[32];

void task_rx(void *p)
{
    int x;
    for (;;)
    {
        lora_receive(); // put into receive mode
        while (lora_received())
        {
            sentdata(UART_NUM_0, "ok \n");
            x = lora_receive_packet(buf, sizeof(buf));
            buf[x] = 0;
            printf("Received: %s\n", buf);
            lora_receive();
        }
        vTaskDelay(1);
    }
}

void RxTask(void *pvParameters)
{
    uint8_t data[128];
    while (1)
    {
        sentdata(UART_NUM_0, "write \n");
        sentdata(UART_NUM_0, &buf);
        delay_s(2);
    }
}

void app_main(void)
{
    lora_init();
    lora_set_frequency(433e6);
    lora_enable_crc();
    start_uart_init(UART_NUM_0);
    xTaskCreate(task_rx, "task_rx", 2048, NULL, 5, NULL);
    xTaskCreate(RxTask, "RxTask", 1024, NULL, 5, NULL);
}
