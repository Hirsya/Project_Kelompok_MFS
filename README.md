<div align="center">
  <p>
<img src="gif_cerpenkita.gif" width="70%" alt="CerpenKita"/><br>
    ⚙️ Project Ini Berjudul <b>CerpenKita</b>.<br>
    💻 Sistem Informasi Sederhana untuk Penikmat dan Penulis Cerita Pendek.<br>
    👷🏻‍♂️ Project Ini Dikerjakan Oleh <b>Kelompok 12 (MFS)</b>.
 
![C++](https://img.shields.io/badge/BAHASA-C%2B%2B-blue%3F?style=for-the-badge&color=blue)
![Interface](https://img.shields.io/badge/INTERFACE-TERMINAL-black%3F?style=for-the-badge&color=orange)
![Status](https://img.shields.io/badge/STATUS-COMPLETE-black%3F?style=for-the-badge&color=green)
  
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
* 💬 **Komentar & Rating:** Pengguna dapat memberi rating (1-5) dan komentar pada cerpen yang sudah dibaca.
* 🎨 **Tampilan Terminal Berwarna:** Menggunakan ANSI Color Code untuk mempercantik tampilan menu dan teks di terminal.
* ⌨️ **Navigasi Arrow Key:** Semua menu menggunakan navigasi panah (↑↓) dan Enter untuk pengalaman yang lebih intuitif.
* ⌨️ **Navigasi Arrow Key:** Semua menu menggunakan navigasi panah (↑↓) dan Enter untuk pengalaman yang lebih intuitif.

---

## 🗂️ 3. Arsitektur Kode

Proyek ini memiliki **2 versi implementasi**:

### **Versi 1: Project_main.cpp** (Backup)
- Menggunakan **array statis + counter** untuk data utama
- Menggunakan `vector` STL untuk helper UI navigation
- Fokus: Simplicity dan rapid development

### **Versi 2: Project_main_pointer.cpp** ⭐ (Rekomendasi)
- **100% pointer dinamis** dengan `new`/`delete`
- Implementasi penuh **Modul 6 (Heap Memory Management)**
- Struktur data:
  * **Dynamic arrays** untuk semua data (Cerpen, Komentar, History, Bookmark)
  * **2D pointer arrays** untuk navigation (history**, Bookmark**, Cerpen**)
  * **Manual memory management** di semua fungsi
  * **Proper cleanup** di semua exit points
- **Arrow Key Navigation** — Semua menu menggunakan navigasi panah untuk UX modern
- **Memory-safe** — No memory leaks, tested dengan multiple scenarios

**Kontributor Proyek (Kelompok 12 - MFS):**

| Nama Kontributor | Tanggung Jawab Utama |
| :--- | :--- |
| **Malikal Hirsya Alfatory** | Membuat variabel `struct` yang dibutuhkan, struktur awal kode, dan koordinasi tim |
| **Ade Zulfikar Rajasa** | Membuat blok **Helper Cerpen** yang mengelola fondasi data utama cerpen |
| **Chandrawati Rachmad** | Sistem **History** dengan dynamic array dan pointer management |
| **Tri Salsabila** | Sistem **Favorit** dengan dynamic array bookmark management |
| **Anak Agung Ayu Intan PM** | Sistem **Komentar & Rating** (1-5) dengan nested array dan arrow navigation |
| **Maulidar Ihlas** | **Logika String & UI** dengan text wrapping dan arrow key navigation |
| **Jhosua Jasson Lada** | **Memory Management** dengan pointer dinamis dan bug fixing memory leaks |

---

## 🛠️ 4. Panduan Menggunakan Program

### ⚙️ Mengcompile Program
C++ merupakan bahasa pemrograman klasik yang mengharuskan kodenya diproses menjadi file aplikasi (`.exe`) agar dapat dibaca dan dijalankan oleh sistem operasi

**Pilihan 1: Versi Pointer (Rekomendasi - Modul 6)**
```bash
g++ Project_main_pointer.cpp -o CerpenKita
```

**Pilihan 2: Versi Original (Backup)**
```bash
g++ Project_main.cpp -o CerpenKita_backup
```

*Keterangan:* Perintah ini menyuruh compiler `g++` untuk membaca file sumber dan membungkusnya menjadi satu file aplikasi.

### 🖥️ Menjalankan Program
Apabila tidak ada pesan error setelah kompilasi selesai, Anda sudah bisa menjalankan programnya:
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
