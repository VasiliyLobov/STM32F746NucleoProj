# Настройка тактирования микроконтроллера на частоте 16 и 216 Мгц и реализация мигания светодиодом на данных частотах
Для мигания на частоте 16 Мгц надо раскомментировать данные строки в файле rcc.c:
//  SystemCoreClockUpdate(); // Функция CMSIS для обновления тактирования
//  return;
