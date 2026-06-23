<div align="center">
  <p>
<img src="gif_cerpenkita.gif" width="70%" alt="CerpenKita"/><br>
    ⚙️ Project Ini Berjudul <b>CerpenKita</b>.<br>
    💻 Sistem Informasi Sederhana untuk Penikmat dan Penulis Cerita Pendek.<br>
    👷🏻‍♂️ Project Ini Dikerjakan Oleh <b>Kelompok 12 (MFS)</b>.
 
![C++](https://img.shields.io/badge/BAHASA-C%2B%2B-blue%3F?style=for-the-badge&color=blue)
![Interface](https://img.shields.io/badge/INTERFACE-TERMINAL-black%3F?style=for-the-badge&color=orange)
![Status](https://img.shields.io/badge/STATUS-REWORK-black%3F?style=for-the-badge&color=red)
  
</div>

---

## 📖 1. Tentang Proyek

**CerpenKita** adalah aplikasi terminal berbasis C++ yang dibuat untuk membantu pengguna membaca, menulis, dan mengelola cerita pendek (cerpen). Sistem ini menggabungkan fitur akun pengguna, daftar bacaan cerpen, riwayat baca, bookmark/favorit, hingga kolom komentar dan rating — semuanya berjalan langsung di terminal tanpa perlu koneksi internet atau database eksternal.

> ⚠️ **Catatan:** Program ini khusus untuk perangkat **Windows**. Saat ini seluruh data (akun, cerpen, history, bookmark, komentar) disimpan **di memori (RAM)** selama program berjalan dan akan **hilang setiap program ditutup**. Belum ada penyimpanan permanen ke file.

---

## 🚀 2. Fitur Utama

* 🔐 **Sistem Akun (Register & Login):** Pendaftaran dan masuk akun dengan input password yang disensor (`*`) menggunakan `_getch()` dari `<conio.h>`.
* 📃 **Daftar & Baca Cerpen:** Menampilkan seluruh cerpen yang tersedia beserta judul, penulis, genre, dan isi cerita.
* ✍️ **Tambah Cerpen:** Pengguna dapat menambahkan cerpen baru ke dalam daftar.
* 🕒 **Riwayat Baca (History):** Mencatat cerpen apa saja yang pernah dibuka oleh pengguna, lengkap dengan waktu baca.
* ⭐ **Favorit:** Menyimpan cerpen favorit agar mudah diakses kembali, dengan opsi tambah/hapus.
* 💬 **Komentar & Rating:** Pengguna dapat memberi rating dan komentar pada cerpen yang sudah dibaca.
* 🎨 **Tampilan Terminal Berwarna:** Menggunakan ANSI Color Code untuk mempercantik tampilan menu dan teks di terminal.

---

## 🗂️ 3. Arsitektur Kode

Berbeda dari proyek pada umumnya, **seluruh logika program (struct, fungsi, dan `main()`) berada dalam satu file tunggal**:
Struktur data yang digunakan:
* **Array** — untuk daftar `akun` pengguna (`daftarAkun[MAX_AKUN]`).

**Kontributor Proyek (Kelompok 12 - MFS):**

| Nama Kontributor | Tanggung Jawab Utama |
| :--- | :--- |
| **Malikal Hirsya Alfatory** | Membuat variabel `struct` yang di butuhkan dan membuat struktur awal kode seperti `main()` |
| **Ade Zulfikar Rajasa** | Membuat blok **Helper Cerpen** yang mengelola fondasi data utama cerpen agar bisa diakses oleh fitur lain. |
| **Chandrawati Rachmad** | Sistem **History** yang fokus mengelola riwayat bacaan yang menempel pada `struct` akun |
| **Tri Salsabila** | Sistem **Favorit** yang fokus mengelola cerpen yang telah di favorit pada `struct` akun |
| **Anak Agung Ayu Intan PM** | Sistem **Komentar & Rating** yang mengelola komentar yang bersarang **(_nested_)** di dalam `struct` Cerpen. |
| **Maulidar Ihlas** | Membuat **Logika String & UI** yang fokus pada manipulasi `string` dan pengalaman membaca agar teks cerpen tidak berantakan di konsol |
| **Jhosua Jasson Lada** | Membuat **Memory Management** yang merajut semua fitur yang sudah dibuat dan memastikan tidak ada _memory leak_ saat program ditutup |

---

## 🛠️ 4. Panduan Menggunakan Program

### ⚙️Mengcompile Program
C++ merupakan bahasa pemrograman klasik yang mengharuskan kodenya diproses menjadi file aplikasi (`.exe`) agar dapat dibaca dan dijalankan oleh sistem operasi

1. Pastikan posisi CMD/PowerShell Anda berada di dalam folder tempat file `Project_main.cpp` berada.
2. Jalankan perintah kompilasi di bawah ini:
```bash
g++ Project_main.cpp -o CerpenKita
```
*Keterangan:* Perintah ini menyuruh compiler `g++` untuk membaca file pusat `Project_main.cpp` (yang otomatis akan menarik file `.cpp` modular lainnya) dan membungkusnya menjadi satu file aplikasi utuh bernama `CerpenKita`.

### 🖥️Menjalankan Program
Apabila tidak ada pesan error setelah kompilasi selesai, Anda sudah bisa menjalankan programnya. Gunakan perintah berikut:
  ```bash
  .\CerpenKita.exe
  ```

## Hal yang tidak penting

<details>
<summary><b>👉 Jangan Ditekan 👈</b></summary>
<br>
<div align="center">
  
**DASAR ORANG KEPOAN...** <br>
Selamat! Kamu Sudah Menemukan *Easter Egg* (yang sama sekali tidak rahasia) di dalam repositori ini. 

<!-- KALAU KALIHAN MEMBACA INI KALIAN ORANG TIDAK SABARAN BANGET DAH GWS-->

<img src="https://media.tenor.com/WyttCDFchNkAAAAM/sparxie-hoyoverse.gif" alt="Yay" width="250"/>

<p>

_sebenarnya ini hal yang tidak penting tapi yasudahlah_ <br>
Selamat dan terimakasih sebesar besarnya dari saya _**Hirsya**_ <br>
Untuk para contributor karena sudah mau membantu saya dalam project ini🥳<br>
_✨Semoga kita semuaa sukses di masa depan✨_
  
</p>

</div>
</details>
