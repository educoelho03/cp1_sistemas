#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

static SemaphoreHandle_t xBinarySemaphore1 = NULL;
static SemaphoreHandle_t xBinarySemaphore2 = NULL;
static SemaphoreHandle_t xBinarySemaphore3 = NULL;
static QueueHandle_t xQueue = NULL;

void Task1(void *pvParameters){
    int vez = 1; 
    while (1) 
    {
        if(xSemaphoreTake(xBinarySemaphore1, portMAX_DELAY) == pdTRUE) 
        {
            printf("[Tarefa 1] Executou - Eduardo\n");
            
            if(vez == 1) 
            {
                xSemaphoreGive(xBinarySemaphore2);
                vez = 2;
            } 
            else 
            {
                xSemaphoreGive(xBinarySemaphore3);
                vez = 1;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); 
}

void Task2(void *pvParameters)
{
    while (1) 
    {
        if(xSemaphoreTake(xBinarySemaphore2, portMAX_DELAY) == pdTRUE) 
        {
            printf("[Tarefa 2] Executou - Eduardo\n");
            xSemaphoreGive(xBinarySemaphore1); 
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void Task3(void *pvParameters)
{
    while (1) 
    {
        if(xSemaphoreTake(xBinarySemaphore3, portMAX_DELAY) == pdTRUE) 
        {
            printf("[Tarefa 3] Executou - Eduardo\n");
            xSemaphoreGive(xBinarySemaphore1);
        }
        vTaskDelay(pdMS_TO_TICKS(1)); 
    }
}

void app_main(void)
{
    printf("Iniciando aplicação FreeRTOS com 3 tarefas intercaladas...\n");

    xBinarySemaphore1 = xSemaphoreCreateBinary();
    xBinarySemaphore2 = xSemaphoreCreateBinary();
    xBinarySemaphore3 = xSemaphoreCreateBinary();

    if (xBinarySemaphore1 == NULL || xBinarySemaphore2 == NULL || xBinarySemaphore3 == NULL) 
    {
        printf("Falha ao criar semáforos binários\n");
        return;
    }

    xQueue = xQueueCreate(5, sizeof(int));
    if(xQueue == NULL) 
    {
        printf("Falha ao criar fila\n");
        return;
    }

    xTaskCreate(Task1, "Task1", 2048, NULL, 5, NULL);
    xTaskCreate(Task2, "Task2", 2048, NULL, 5, NULL);
    xTaskCreate(Task3, "Task3", 2048, NULL, 5, NULL);

    xSemaphoreGive(xBinarySemaphore1);

    printf("Tarefas criadas e semáforos inicializados!\n");
}