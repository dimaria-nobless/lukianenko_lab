МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ<br/>
Федеральное государственное автономное образовательное учреждение высшего образования<br/>
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"<br/>
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ<br/>
Кафедра компьютерной инженерии и моделирования<br/>
<br/><br/>

### Отчёт по лабораторной работе № 6<br/> по дисциплине "Программирование"

<br/>

студента 1 курса группы ИВТ-б-о-191(2)<br/>
Лукьяненко Дмитрия Владимировича<br/>
направления подготовки 09.03.01 "Информатика и вычислительная техника"<br/>
<br/>

<table>
<tr><td>Научный руководитель<br/> старший преподаватель кафедры<br/> компьютерной инженерии и моделирования</td>
<td>(оценка)</td>
<td>Чабанов В.В.</td>
</tr>
</table>

<br/><br/>

Симферополь 2020


# Лабораторная работа №6
## Погодный информер

### Цель: 

1. Закрепить навыки разработки многофайловыx приложений;
2. Изучить способы работы с API web-сервиса;
3. Изучить процесс сериализации/десериализации данных.

### Ход работы:
1. Программа

   ```c++
   #include <iostream>
   #include <fstream>
   #include <httplib/httplib.h>
   #include <nlohmann/json.hpp>
   #include <string>
   using namespace httplib;
   using namespace std;
   using json = nlohmann::json;
   void replace(string& str, const string from,string  to, int key)
   {
       if (key == 1) to = to.substr(1, 10);
       else if (key == 2) to = to.substr(1, 3);
   
       int startPos = str.find(from);
       if (startPos == string::npos) return;
   
       str.replace(startPos, from.length(), to);
   }
   
   void gen_response(const Request& req, Response& result)
   { 
       string widget;
       Client clientik("api.openweathermap.org", 80);
       auto res = clientik.Get("/data/2.5/forecast?id=524901&units=metric&APPID=1f45d3f3cec033897d1714f0200a1e85");
       json j;
       if (res && res->status == 200) j = json::parse(res->body);
       ifstream file("informer_template.html");
       if (!file) {
           cout << "File not open"; 
       }
       else{
       getline(file, widget, '\0');
       int digit = 0;
       file.close();
       replace(widget, "{city.name}", j["city"]["name"].dump(), 1);
           int start = j["list"][0]["dt"];
           int tip = 0, 
               top = 0; 
           while (tip!=5) {
               top++;
               if (j["list"][top]["dt"] >= start)
               {
                   replace(widget, "{list.dt}", j["list"][top]["dt_txt"].dump(), 1);
                   replace(widget, "{list.weather.icon}", j["list"][top]["weather"][0]["icon"].dump(), 2);
                   replace(widget, "{list.main.temp}", j["list"][top]["main"]["temp"].dump(), 0);
                   start += 86400;
                   tip++;
               }
           }
       result.set_content(widget, "text/html");
       }
   }
   
   
   int main()
   {
       httplib::Server svr;
       svr.Get("/", gen_response);
       svr.listen("localhost", 3000); 
   }
   ```
2. Заполненный виджет
   
   
<img src="file:///C:\Users\Маргарита\lukianenko_lab\Screenshot_6.1.png" />
​                                                                              Рисунок 1. Заполненный виджет

# Вывод:

Во время выполнения лабораторной работы я закрепил навыки разработки многофайловыx приложений, изучил способы работы с API web-сервиса, изучил процесс сериализации/десериализации данных.


