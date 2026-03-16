**ENGLISH**

# 🚀 NASA ECLSS (Environmental Control and Life Support System) Simulation

This project is a deterministic simulation written in C that models the behavior of a space station's **Environmental Control and Life Support System (ECLSS)**. The goal of the project is to demonstrate practical applications of embedded systems design, system simulation, and secure C programming techniques.

The simulation models how the following systems interact over time:
* Oxygen generation and Carbon dioxide removal
* Crew metabolism
* Environmental sensors and Alarm system
* Hardware failure scenarios

---

## 🧠 System Overview

The simulation runs on a time-based loop, updating environmental values every hour. The main simulated components include:
* Oxygen level control
* Carbon dioxide accumulation and scrubbing
* Pressure, temperature, and humidity monitoring
* Metabolic gas production by the crew
* Operation and failure of hardware modules
* Alarm monitoring system

---

## ⚙️ Simulated Hardware Modules

### 1. Oxygen Generation System (OGS)
The OGS module generates the oxygen consumed by the crew and attempts to balance the O₂ levels in the station.
* Has a fixed oxygen generation capacity.
* Production rate can vary based on system efficiency.
* The module can be autonomously disabled or experience a malfunction.

### 2. Carbon Dioxide Removal Assembly (CDRA)
The CDRA module scrubs toxic carbon dioxide from the environment.
* Has a specific hourly CO₂ removal capacity.
* Operates based on system efficiency and is prone to malfunctions.
* If the CDRA fails, the CO₂ level in the environment will begin to rise continuously.

---

## 👨‍🚀 Crew Metabolism and Atmosphere

There are two crew members in the simulation: **Cmdr. Yilmaz** and **Eng. Baran**. Each member consumes oxygen and produces carbon dioxide. These gas exchange rates change dynamically based on their current activity type:
* **Sleeping:** Low metabolism (Low consumption/production)
* **Researching:** Normal metabolism
* **Exercise:** High metabolism (High consumption/production)

### 🌍 Monitored Atmospheric Parameters
The alarm system autonomously engages when values fall outside safe boundaries:
* Oxygen level (%)
* Carbon dioxide level (%)
* Pressure (kPa)
* Temperature (°C)
* Humidity (%)
* Water reserve (Liters)

---

## 🚨 Alarm System and Threat Levels

The alarm system is designed using a **bitmask** architecture. Thanks to `union` and `bit-field` structures, all alarms are packed into a **single byte**. This allows the system state to be stored using minimal memory (RAM).

Each bit represents a different hazard (Low/High O₂, High CO₂, dangerous pressure/temperature/humidity, low water). By reading these bits, the system shifts into one of 4 different threat levels:

| Threat Level | Description |
| :--- | :--- |
| `SYSTEM_NORMAL` | The system is operating normally and is stable. |
| `SYSTEM_WARNING` | Minor environmental issues detected. |
| `SYSTEM_CRITICAL` | A dangerous situation is developing, intervention may be required. |
| `SYSTEM_EMERGENCY` | Immediate and life-threatening risk for the crew! |

---

## ⏱ Simulation Scenario (30-Hour Test)

The program simulates a total mission duration of 30 hours. To test how the system behaves under stress and to evaluate the alarm mechanism, two critical events occur during the mission:

* **T+03 Hours:** Engineer Baran begins heavy exercise. This suddenly increases oxygen consumption and CO₂ production.
* **T+06 Hours:** The CDRA (CO₂ scrubber) module fails. CO₂ cannot be removed from the environment, and toxic gas levels begin to rise rapidly.

---

## 📊 Telemetry Log (Black Box)

While running, the program automatically generates a telemetry (log) file. This file, named `telemetry_log.txt`, acts like a simplified spacecraft black box and stores the following data:
* Mission time
* Oxygen and CO₂ percentages
* System threat level and Alarm byte value (Hexadecimal)

---

## 🧩 Programming Techniques Utilized

* **Bitmask Alarm System:** Hardware-level alarm control.
* **Union and Bit-field Data Structures:** Advanced memory optimization.
* **Secure String Operations:** Use of `strncpy` for Buffer Overflow protection.
* **Modular Function Design & Time-Based Simulation**

---

## ▶️ How to Run

You can easily compile and run the project using the GCC compiler:

bash
# To compile the code
gcc eclss_simulation.c -o eclss

# To start the simulation
./eclss

============================================================================


# 🚀 NASA ECLSS (Yaşam Destek Sistemi) Simülasyonu

Bu proje, bir uzay istasyonunun **Çevresel Kontrol ve Yaşam Destek Sistemi (ECLSS)** davranışını modelleyen, C dili ile yazılmış deterministik bir simülasyondur. Projenin amacı; gömülü sistem tasarımı, sistem simülasyonu ve güvenli C programlama tekniklerini uygulamalı olarak göstermektir.

Simülasyon aşağıdaki sistemlerin zaman içinde nasıl etkileşime girdiğini modeller:
* Oksijen üretimi ve Karbondioksit temizleme
* Mürettebat metabolizması
* Çevresel sensörler ve Alarm sistemi
* Donanım arıza senaryoları

---

## 🧠 Sistem Genel Yapısı

Simülasyon, zaman tabanlı bir döngü ile çalışır ve her saat çevresel değerleri günceller. Simüle edilen ana bileşenler şunlardır:
* Oksijen seviyesi kontrolü
* Karbondioksit birikimi ve temizlenmesi
* Basınç, sıcaklık ve nem takibi
* Mürettebatın metabolik gaz üretimi
* Donanım modüllerinin çalışması veya arızalanması
* Alarm izleme sistemi

---

## ⚙️ Simüle Edilen Donanım Modülleri

### 1. Oksijen Üretim Sistemi (OGS)
OGS modülü, mürettebatın tükettiği oksijeni üretir ve istasyondaki O₂ seviyesini dengelemeye çalışır.
* Sabit oksijen üretim kapasitesine sahiptir.
* Sistem verimliliğine göre üretim miktarı değişebilir.
* Modül otonom olarak devre dışı bırakılabilir veya arızalanabilir.

### 2. Karbondioksit Temizleme Sistemi (CDRA)
CDRA modülü ortamdaki zehirli karbondioksiti temizler.
* Saatlik spesifik bir CO₂ temizleme kapasitesi vardır.
* Sistem verimliliğine bağlı çalışır ve arızalanabilir.
* CDRA arızalanırsa ortamdaki CO₂ seviyesi sürekli artmaya başlar.

---

## 👨‍🚀 Mürettebat Metabolizması ve Atmosfer

Simülasyonda **Cmdr. Yilmaz** ve **Eng. Baran** olmak üzere iki mürettebat bulunmaktadır. Her üye oksijen tüketir ve karbondioksit üretir. Bu gaz alışveriş oranları anlık aktivite türüne göre dinamik olarak değişir:
* **Uyku:** Düşük metabolizma (Düşük tüketim/üretim)
* **Araştırma:** Normal metabolizma
* **Egzersiz:** Yüksek metabolizma (Yüksek tüketim/üretim)

### 🌍 Takip Edilen Atmosfer Parametreleri
Değerler güvenli sınırların dışına çıktığında alarm sistemi otonom olarak devreye girer:
* Oksijen seviyesi (%)
* Karbondioksit seviyesi (%)
* Basınç (kPa)
* Sıcaklık (°C)
* Nem (%)
* Su rezervi (Litre)

---

## 🚨 Alarm Sistemi ve Tehdit Seviyeleri

Alarm sistemi **bitmask (bit maskeleme)** mimarisi kullanılarak tasarlanmıştır. `union` ve `bit-field` yapıları sayesinde tüm alarmlar **tek bir byte** içinde tutulur. Bu sayede sistem durumu çok düşük bellek (RAM) kullanılarak saklanır.

Her bit farklı bir tehlikeyi temsil eder (Düşük/Yüksek O₂, Yüksek CO₂, tehlikeli basınç/sıcaklık/nem, düşük su). Sistem bu bitleri okuyarak 4 farklı tehdit seviyesinden birine geçer:

| Tehdit Seviyesi | Açıklama |
| :--- | :--- |
| `SYSTEM_NORMAL` | Sistem normal ve stabil çalışıyor. |
| `SYSTEM_WARNING` | Küçük çevresel problemler tespit edildi. |
| `SYSTEM_CRITICAL` | Tehlikeli durum gelişiyor, müdahale gerekebilir. |
| `SYSTEM_EMERGENCY` | Mürettebat için acil ve hayati risk! |

---

## ⏱ Simülasyon Senaryosu (30 Saatlik Test)

Program toplam 30 saatlik bir görev süresini simüle eder. Görev sırasında sistemin stres altında nasıl davrandığını ve alarm mekanizmasını test etmek için iki kritik olay gerçekleşir:

* **T+03. Saat:** Mühendis Baran ağır egzersize başlar. Bu durum aniden oksijen tüketimini ve CO₂ üretimini artırır.
* **T+06. Saat:** CO₂ temizleyici sistem olan CDRA arızalanır. Ortamdan CO₂ temizlenemez ve toksik gaz seviyesi hızla yükselmeye başlar.

---

## 📊 Telemetri Kaydı (Kara Kutu)

Program çalışırken otomatik olarak bir telemetri (log) dosyası oluşturur. `telemetry_log.txt` adındaki bu dosya, basitleştirilmiş bir uzay aracı kara kutusu gibi çalışır ve şu verileri saklar:
* Görev zamanı
* Oksijen ve CO₂ yüzdesi
* Sistem tehdit seviyesi ve Alarm byte değeri (Hexadecimal)

---

## 🧩 Kullanılan Programlama Teknikleri

* **Bitmask Alarm Sistemi:** Donanım seviyesinde alarm kontrolü.
* **Union ve Bit-field Veri Yapıları:** İleri düzey bellek optimizasyonu.
* **Güvenli String İşlemleri:** Buffer Overflow (Bellek Taşması) koruması için `strncpy` kullanımı.
* **Modüler Fonksiyon Tasarımı & Zaman Tabanlı Simülasyon**

---

## ▶️ Nasıl Çalıştırılır?

Projeyi GCC derleyicisi kullanarak kolayca derleyebilir ve çalıştırabilirsiniz:

bash
# Kodu derlemek için
gcc eclss_simulation.c -o eclss


# Simülasyonu başlatmak için
./eclss
