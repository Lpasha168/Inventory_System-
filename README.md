# Inventory_System-
**Описание полей:**
| Поле | Тип | Описание | 
| :--- | :--- | :--- | 
| id | INTEGER | Уникальный идентификатор | 
| SKU | TEXT | Название товара | 
| quantity | INTEGER | Количество товара | 
| price | REAL | Цена товара |
| Category | TEXT | Категория товара |
| last_update | DATETIME | Время обновления |
***
**Документация API:**

GET /inventory: получить список товаров

POST /inventory: добавить товар

DELETE /inventoey: удалить товар

POST /inventory/{id}/sell: уменьшить количество 
***
**Запуск:**

В ***корневой*** папке проекта запустить командую строку и вставить по очереди команды:

`mkdir build`

`cd build`

`cmake ..`

`cmake --build .`

После этого **server.exe** должен появиться в папке `Inventory_System\build\server\Debug`. Запускаем его

