#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>
using namespace std;

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_RED "\033[1;31m"
#define COLOR_CYAN "\033[1;36m"
#define COLOR_GRAY "\033[0;90m"
#define COLOR_WHITE "\033[1;37m"
#define COLOR_MAGENTA "\033[1;35m"

// ─── Structs ─────────────────────────────────────────────────

struct Cerpen
{
  int id;
  string judul;
  string penulis;
  string genre;
  string isi;
  Cerpen *next;
};

struct Komentar
{
  int rating;
  string nama;
  string isi;
  string waktu;
  Komentar *next;
};

struct history
{
  string judul;
  string waktu;
  history *next;
  int idHistory;
};

struct Bookmark
{
  string judul;
  Bookmark *next;
  int idBookmark;
};

struct akun
{
  string nama;
  string password;
  history *riwayat;
  Bookmark *bookmark;
  int jumlahRiwayat;
  int jumlahBookmark;
};

// ─── Global ──────────────────────────────────────────────────

const int MAX_AKUN = 100;
akun daftarAkun[MAX_AKUN];
int jumlahAkun = 0;

Cerpen *headCerpen = nullptr;
int jumlahCerpen = 0;

// ─── Helper ──────────────────────────────────────────────────

void clearScreen() { system("cls"); }

void tampilHeader(const string &judul)
{
  cout << COLOR_CYAN
       << "|======================================|\n"
       << "|" << COLOR_WHITE;
  int pad = (38 - (int)judul.size()) / 2;
  for (int i = 0; i < pad; i++)
    cout << ' ';
  cout << judul;
  for (int i = 0; i < 38 - pad - (int)judul.size(); i++)
    cout << ' ';
  cout << COLOR_CYAN << "|\n"
       << "|======================================|\n"
       << COLOR_RESET;
}

string inputPassword()
{
  string pass;
  char ch;
  while ((ch = _getch()) != '\r')
  {
    if (ch == '\b' && !pass.empty())
    {
      pass.pop_back();
      cout << "\b \b";
    }
    else if (ch != '\b')
    {
      pass += ch;
      cout << '*';
    }
  }
  cout << '\n';
  return pass;
}

string getCurrentTimestamp()
{
  time_t now = time(nullptr);
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtime(&now));
  return string(buffer);
}

// ─── Auth Logic ──────────────────────────────────────────────

bool namaAkunAda(const string &nama)
{
  for (int i = 0; i < jumlahAkun; i++)
    if (daftarAkun[i].nama == nama)
      return true;
  return false;
}

int cariAkun(const string &nama, const string &password)
{
  for (int i = 0; i < jumlahAkun; i++)
    if (daftarAkun[i].nama == nama && daftarAkun[i].password == password)
      return i;
  return -1;
}

// ─── Menu Utama Setelah Login ─────────────────────────────────

void menuUtama(int indexAkun)
{
  clearScreen();
  tampilHeader("SELAMAT DATANG");
  cout << COLOR_GREEN << "\n  Halo, " << daftarAkun[indexAkun].nama << "!\n"
       << COLOR_RESET;

  // Fitur nanti disini untuk kita kerjain masing" setelah bagi tugas

  cout << COLOR_GRAY << "  (fitur baca cerpen akan hadir di sini)\n"
       << COLOR_RESET;
  cout << "\n  Tekan Enter untuk kembali...";
  cin.ignore();
  cin.get();
}

// ─── Register ────────────────────────────────────────────────

void menuRegister()
{
  clearScreen();
  tampilHeader("REGISTER");

  if (jumlahAkun >= MAX_AKUN)
  {
    cout << COLOR_RED << "\n  Kapasitas akun penuh!\n"
         << COLOR_RESET;
    cout << "  Tekan Enter...";
    cin.ignore();
    cin.get();
    return;
  }

  string nama, password;

  cout << "\n  " << COLOR_GRAY << "(ketik 0 untuk kembali)\n"
       << COLOR_RESET;
  cout << "\n  Nama pengguna : ";
  cin >> nama;

  if (nama == "0")
    return;

  if (namaAkunAda(nama))
  {
    cout << COLOR_RED << "\n  Nama sudah dipakai. Coba nama lain.\n"
         << COLOR_RESET;
    cout << "  Tekan Enter...";
    cin.ignore();
    cin.get();
    return;
  }

  cout << "  Password      : ";
  cin.ignore();
  password = inputPassword();

  if (password.empty())
  {
    cout << COLOR_RED << "\n  Password tidak boleh kosong!\n"
         << COLOR_RESET;
    cout << "  Tekan Enter...";
    cin.get();
    return;
  }

  daftarAkun[jumlahAkun] = {nama, password, nullptr, nullptr, 0, 0};
  jumlahAkun++;

  cout << COLOR_GREEN << "\n  Akun berhasil dibuat! Silakan login.\n"
       << COLOR_RESET;
  cout << "  Tekan Enter...";
  cin.get();
}

// ─── Login ───────────────────────────────────────────────────

void menuLogin()
{
  clearScreen();
  tampilHeader("LOGIN");

  string nama, password;

  cout << "\n  " << COLOR_GRAY << "(ketik 0 untuk kembali)\n"
       << COLOR_RESET;
  cout << "\n  Nama pengguna : ";
  cin >> nama;

  if (nama == "0")
    return;

  cout << "  Password      : ";
  cin.ignore();
  password = inputPassword();

  int idx = cariAkun(nama, password);
  if (idx == -1)
  {
    cout << COLOR_RED << "\n  Nama atau password salah!\n"
         << COLOR_RESET;
    cout << "  Tekan Enter...";
    cin.get();
    return;
  }

  cout << COLOR_GREEN << "\n  Login berhasil!\n"
       << COLOR_RESET;
  menuUtama(idx);
}

// ─── Main ─────────────────────────────────────────────────────

int main()
{
  // Data dummy akun
  daftarAkun[0] = {"hirsya", "123", nullptr, nullptr, 0, 0};
  daftarAkun[1] = {"ridho", "321", nullptr, nullptr, 0, 0};
  jumlahAkun = 2;

  int pilihan;

  while (true)
  {
    clearScreen();
    tampilHeader("RUMAH BACA CERPEN");

    cout << COLOR_YELLOW
         << "\n  [1] Login\n"
         << "  [2] Register\n"
         << COLOR_RED
         << "  [3] Keluar\n"
         << COLOR_RESET
         << "\n  Pilih: ";

    if (!(cin >> pilihan))
    {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }

    switch (pilihan)
    {
    case 1:
      menuLogin();
      break;
    case 2:
      menuRegister();
      break;
    case 3:
      clearScreen();
      cout << COLOR_GREEN << "  Sampai jumpa!\n\n"
           << COLOR_RESET;
      return 0;
    default:
      cout << COLOR_RED << "\n  Pilihan tidak valid!\n"
           << COLOR_RESET;
      cout << "  Tekan Enter...";
      cin.ignore();
      cin.get();
    }
  }
}
