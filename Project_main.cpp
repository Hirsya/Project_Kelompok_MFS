#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>
#include <vector>
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
struct Komentar
{
  int rating;
  string nama;
  string isi;
  string waktu;
  Komentar *next;
};

struct Cerpen
{
  int id;
  string judul;
  string penulis;
  string genre;
  string isi;
  string waktu;
  Cerpen *next;
  Komentar *komentarHead;
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

string normalkanKarakter(const string &teks)
{
  struct Ganti { string dari, ke; };
  Ganti tabel[] = {
    {"\xE2\x80\x9C", "\""},  // " kiri
    {"\xE2\x80\x9D", "\""},  // " kanan
    {"\xE2\x80\x98", "'"},   // ' kiri
    {"\xE2\x80\x99", "'"},   // ' kanan
    {"\xE2\x80\x93", "-"},   // en dash
    {"\xE2\x80\x94", "--"},  // em dash
    {"\xE2\x80\xA6", "..."}  // ellipsis
  };
  string hasil = teks;
  int n = (int)(sizeof(tabel) / sizeof(tabel[0]));
  for (int i = 0; i < n; i++) {
    size_t pos = 0;
    while ((pos = hasil.find(tabel[i].dari, pos)) != string::npos) {
      hasil.replace(pos, tabel[i].dari.size(), tabel[i].ke);
      pos += tabel[i].ke.size();
    }
  }
  return hasil;
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

// ─── Cerpen Helper ───────────────────────────────────────────

Cerpen *cariCerpenById(int id)
{
  Cerpen *cur = headCerpen;
  while (cur != nullptr)
  {
    if (cur->id == id)
      return cur;
    cur = cur->next;
  }
  return nullptr;
}

Cerpen *cariCerpenByJudul(const string &judul)
{
  Cerpen *cur = headCerpen;
  while (cur != nullptr)
  {
    if (cur->judul == judul)
      return cur;
    cur = cur->next;
  }
  return nullptr;
}

string buatPreview(const string &isi, int maxLen = 100)
{
  string normal = normalkanKarakter(isi);
  if ((int)normal.size() <= maxLen)
    return normal;
  return normal.substr(0, maxLen) + "...";
}

double hitungRatingRata(Cerpen *c)
{
  if (c->komentarHead == nullptr)
    return 0;

  int total = 0, jumlah = 0;
  Komentar *cur = c->komentarHead;
  while (cur != nullptr)
  {
    total += cur->rating;
    jumlah++;
    cur = cur->next;
  }
  return (double)total / jumlah;
}

void tambahCerpen(int id, string judul, string penulis, string genre, string isi)
{
  Cerpen *baru = new Cerpen{id, judul, penulis, genre, isi, getCurrentTimestamp(), nullptr, nullptr};

  if (headCerpen == nullptr)
  {
    headCerpen = baru;
  }
  else
  {
    Cerpen *cur = headCerpen;
    while (cur->next != nullptr)
      cur = cur->next;
    cur->next = baru;
  }
  jumlahCerpen++;
}

void seedCerpen()
{
  tambahCerpen(1, "Senja di Ujung Jalan", "Hirsya", "Drama",
               "Senja itu, Rian berdiri sendirian di ujung jalan yang sepi. Ia menatap langit yang perlahan berubah jingga, mengingat kembali semua kenangan yang pernah ia lalui di kota ini. Sebentar lagi ia akan pergi, meninggalkan semuanya untuk memulai hidup baru di kota lain.");

  tambahCerpen(2, "Surat Untuk Ayah", "Ridho", "Keluarga",
               "Sudah lima tahun sejak ayah meninggal, namun surat itu baru kutemukan hari ini, terselip di antara buku-buku tua di gudang. Tanganku gemetar saat membuka lipatan kertas yang sudah menguning itu. Di dalamnya, ayah menuliskan harapan-harapan yang tak pernah ia ucapkan secara langsung.");

  tambahCerpen(3, "Misteri Rumah Tua", "Maya", "Horor",
               "Rumah tua di ujung desa itu sudah lama ditinggalkan penghuninya. Konon, setiap malam Jumat Kliwon, terdengar suara tangisan dari dalam rumah tersebut. Malam itu, empat orang remaja memutuskan untuk membuktikan kebenarannya, tanpa tahu apa yang menanti mereka di dalam.");

  tambahCerpen(4, "Nala dan Mimpi Buruk", "Lala", "Fantasi", R"(Nala adalah seorang gadis berusia 12 tahun, berparas cantik dan memiliki lesung pipit 
dikedua pipinya ketika ia tersenyum. Namun sayang, Nala memiliki sifat yang tidak 
cocok dengan parasnya itu, Nala sangatlah keras kepala, ia tidak suka dan tidak mau 
mendengarkan orang lain, parahnya ia suka sekali melakukan hal yang bisa merusak 
lingkungan, seperti membuang sampah sembarangan dan merusak tanaman apapun 
yang ia rasa mengganggu penglihatannya. 
Suatu hari tepat saat bel pulang sekolah berbunyi, seperti biasa Nala dan sahabatnya 
Vanessa akan pulang bersama menggunakan sepedanya. Namun saat dipertengahan 
jalan ban sepeda Nala bocor sehingga ia tidak bisa menggunakan sepedanya itu. 
“Aduuh, kenapa pakai bocor segala siiih bannya? Mana panas banget lagi! Huh” keluh 
Nala. 
“Gak boleh ngeluh gitu atuh Naa, lagian situ udah dibilangin jangan lewat sana, banyak 
kerikil tajamnnya. Masiih aja dilewatin” tegur Vanessa. 
Nala tidak menghiraukan perkataan sahabat kecilnya itu, ia masih kesal. Ia pun lanjut 
berjalan dengan terpaksa mendorong sepedanya, Vanessa yang merasa kasihan dengan 
Nala pun ikut mendorong sepedanya mengikuti Nala. Selang beberapa menit kemudian 
mereka berhenti di tempat tambal ban didekat taman. 
“Pakdee, tolong ini di tambal ya! Saya mau beli es dulu di warung depan” ucap Nala 
seraya meletakan sepedanya di tempat yang sudah disediakan. 
“siap neng” balas Pakde. 
Nala dan Vanessa pun berjalan menuju warung dan membeli dua es jeruk untuk mereka 
minum. Sambil menunggu sepeda Nala ditambal, mereka bermain di taman dekat sana. 
Nala yang masih sedikit kesal mulai mencabuti tanaman-tanaman yang ada di taman 
tersebut, mulai dari bunga-bunga, rerumputan, hingga pohon yang masih baru ditanam 
oleh tukang pun ia rusak. 
“NALA! Jangan dirusak gituu atuh tanaman-tanamannya. Kan itu udah ditanam sama 
tukang-tukang biar tamannya sejuk dan asri” omel Vanessa. 
“Ish, tapi aku masi kesel Van. Aku butuh melampiaskan rasa kekesalanku” . 
“Tapi jangan gitu juga doong, gak boleh merusak lingkungan! Kamu lupa sama apa 
kata bu guru di sekolah? Nanti-” perkataan Vanessa tepotong. 
“udah deh Van, buktinya aku selalu melakukan hal ini, gak terjadi apa-apa tuh!. udahlah 
ayo balik, aku makin kesel denger kamu ngomel terus” Nala beranjak dari tempatnya 
menuju tambal ban untuk mengambil sepedanya, ia membuang plastik esnya 
disembarang tempat. 
“ NALAAA! JANGAN BUANG SAMPAH SEMBARANGAN, HEI!” teriak Vanessa 
dari bangku taman, Nala tak menghiraukan, ia malah melambaikan tanggannya sambil 
berjalan meninggalkan Vanessa. Akhirnya Vanessa lah yang mebuang sampah tersebut 
ke tempat sampah, dan langsung berlari menyusul Nala. ------ 
Sampai dirumah Nala segera mengganti seragamnya dengan pakaian rumah yang biasa 
ia gunakan, mencuci tangan, dan makan siang. Setelah nya Nala kembali ke kamarnya 
untuk tidur siang. 
“NALA! BANGUN SAYANG, CEPAT” terdengar teriakan sang bunda 
membangunkan Nala dari alam mimpinya. 
“kenapa sih buun?? Nala masih ngantuk ini…hoaaam” ujar Nala namun matanya masih 
terpejam. 
“Ayo segera sayang, air sudah mulai tinggi. Kita harus segera evakuasi!” balas bunda 
terdengar panik, menarik tangan Nala menuju pintu keluar. 
Nala terkejut, ia melihat seluruh desanya sudah digenangi air yang cukup tinggi hingga 
sebatas pahanya. Rasa kantuknya tiba-tiba menghilang, ia mulai panik. Segera ia 
menaiki perahu yang sudah menunggu didepan rumahnya, ia melihat sang ayah 
membawa beberapa barang penting keluar dari rumahnya. Nala tak bisa berkata apa
apa, ia hanya terdiam melihat ke sekitar. Ia terheran, kenapa ia tidak bisa melihat pohon
pohon tinggi yang ada didesanya saat itu, semua yang bisa ia lihat adalah langit yang 
menggelap karena matahari sudah mulai menghilang bak ikut tenggelam kedalam air. 
Seketika suasana terasa hening, hanya terdengar lantunan pelan sang Bunda yang 
membaca doa agar mereka bisa sampai di tempat evakuasi dengan selamat sore itu. 
Sekitar 40 menitan mereka diatas air, akhirnya mereka sampai di tempat evakuasi yang 
berada dibukit yang cukup tinggi. Nala turun begitu pula dengan Ayah dan Bundanya.  
Nala berkeliling di lokasi tenda-tenda, mencari sahabatnya. Ia melihat siluet tante Anita, 
Bunda Vanessa, berlari menuju sebuah tenda berwarna orange menyala. Nala merasa 
ada sesuatu yang mengganjal, membuatnya mengikuti Tante Anita dari belakang.  
Kaget, sedih, panik. Itu yang ia rasakan, hatinya sesak melihat sahabat baiknya itu 
tergeletak kaku di atas terpal. Ia mendekat secara perlahan, seperti sedang membawa 
sesuatu yang rentan dan mudah hancur di dalam dirinya. Kakinya gemetar, air mata 
mulai menutupi pandangannya. 
“.… Van? Vanessa aurelia, kamu kenapa? Tan- hiks, Tante, Vanessa kenapa Tan? 
Vanessa gak apa-apa kan Tan? Dia cuma tidur aja kan? Tapi- hiks, kenapa tubuh 
Vanessa dingin? Padahal disini gak dingin”  Nala terisak, ia mulai meracau 
mengeluarkan pertanyaan-pertanyaan yang timbul di benaknya. Bulir air matanya 
sudah jatuh entah sejak kapan. 
“... tenggelam… Vanessa tenggelam” tutur Tante Anita pelan. 
“ ia ditemukan didekat taman, sudah tidak sadarkan diri, dan saat dibawa kesini, 
dia..dia.. sudah pergi, jauh meninggalkan Tante” lanjutnya. 
Nala tersentak, tidak percaya dengan apa yang barusan ia dengar. Ia berlari keluar dari 
tenda dengan air matanya yang sudah deras mengalir. Terduduk di bagian bukit yang 
sepi, menatap langit malam sambil menangis. Ia membatin, kenapa tuhan jahat sekali 
kepadanya? Apa yang telah ia lakukan hingga berakibat seperti ini?. seketika terputar 
ingatannya tentang kejadian tadi siang, ketika ia kesal, dan merusak tanaman- tanaman 
di taman, dan ia yang membuang sampah sembarangan. Memori lain ikut terputar, 
tentang perkataan gurunya disekolah. 
“Anak-anak yang ibu cintai, kalian jangan pernah membuang sampah sembarangan 
ya. Karena itu akan mengakibatkan saluran air tersumbat dan bisa terjadi banjir. Juga 
jangan merusak tanaman-tanaman di sekeliling kalian, selain untuk memperindah 
suasana, tanaman juga berfungsi untuk mencegah banjir”. 
Tangisan Nala semakin kencang, ia menyesal tidak mendengarkan perkataan Vanessa, 
ia menyesal telah merusak banyak tumbuhan di sekitarannya. Ia menyesal, berpikir 
bahwa semua ini adalah salah nya, Vanessa pergi karena ketidak peduliannya terhadap 
lingkungannya. Ia kemudian berdoa kepada tuhan agar memaafkan segala hal tidak baik 
yang telah ia lakukan kepada lingkungannya. Kemudian pandangannya menggelap. ------ 
tit..tiit..tiit.. 
Nala terbangun dari tidurnya, ia membeku. Tak lama ia melihat ke sekeliling kamarnya, 
beranjak turun dan melihat keluar, burung-burung berterbangan untuk kembali pulang 
ke sangkarnya. Angin bertiup lembut, pohon bergerak tertiup hembusan angin, dan 
langit berwarna jingga keemasan yang indah. 
“tidak ada air” batinnya. 
Ia segera kembali kekamarnya, mengambil smartphone miliknya dari atas meja 
belajarnya. Menekan kontak yang tertulis ‘Vanessa cerewet’ dan benda itu mulai 
berdering. Perasaan gelisah di hatinya, berharap sang sahabat mengangkat teleponnya. 
‘tuk’ 
“Halo Nala? Ada apa?” terdengar suara halus menyapa telinganya. 
“Kamu.. kamu gak apa-apa kan?”  
“huh? Iya aku gak apa-apa kok, kenapa emangnya?” 
“tidak, tidak ada. Aku hanya ingin minta maaf ke kamu. Ya sudah aku matikan ya. 
Daah” Nala mematikan sambungan teleponnya, ia lega, semua kejadian yang ia alami 
hanyalah mimpi buruk tidur siangnya. Ia berjanji, mulai saat ini dan seterusnya ia tidak 
akan pernah melakukan kesalahan yang sama lagi seperti dirinya yang dulu, Nala akan 
menjaga lingkungan sekitarnya dengan baik agar mimpi buruk yang ia dapatkan tidak 
menjadi kenyataan mengerikan yang menghancurkan segalanya.)");
  
  tambahCerpen(5,"A Letter Z","JabReal", "Romance, Slice of Life" ,R"(Bagi Irfan, malam hari adalah puncak kehidupan dengan deru mesin motor di jalanan, dan siang hari di kampus hanyalah formalitas melelahkan di depan layar monitor. Sebagai mahasiswa Sistem Informasi, otaknya mungkin dipenuhi logika kode pemrograman, tapi penampilannya sama sekali tidak mencerminkan anak IT pada umumnya. Dengan jaket denim pudar, rambut agak berantakan, dan langkah kaki yang santai, ia menyusuri koridor lantai dua gedung fakultas hari itu.
Bruk!
"Aduh! Omo, omo! Laptopku, buku fikihku!"
Irfan menghentikan langkah. Di depannya, seorang cewek sudah terduduk di lantai koridor. Buku-buku tebal dan sebuah laptop berserakan di sekitarnya. Yang pertama kali menarik perhatian Irfan adalah kacamata bulat yang bertengger agak miring di hidung cewek itu, serta jilbab instan rapi yang kini sedikit bergeser.
"Jalan pakai mata, dong," gumam Irfan refleks, ketus seperti biasa.
Cewek itu mendongak, membetulkan letak kacamatanya dengan cepat. Bukannya takut melihat tampang Irfan yang urakan, ia justru menatap Irfan dengan mata berbinar sesaat sebelum mengembungkan pipinya kesal.
"Di mana-mana jalan itu pakai kaki, Kak. Mata itu untuk melihat. Kalau jalan pakai mata, nanti pusing!" sahut cewek itu dengan nada polos tapi penuh penekanan. "Lagian, tabrakan tadi tuh enggak ada romantis-romantisnya kayak di drakor Crash Landing on You. Sakit tahu! Untung laptopku enggak error, kalau datanya hilang emang Kakak mau tanggung jawab?" gumamnya pelan sambil mengusap lutut.
Irfan tertegun. Belum pernah ada cewek di kampus ini yang berani mendebatnya dengan argumen se-aneh itu, ditambah membawa-bawa istilah drama Korea dan mengkhawatirkan data laptop di depan seorang anak Sistem Informasi.
Cewek itu segera bangkit, dengan gesit mengumpulkan barang-barangnya. "Maaf ya, Kak. Saya buru-buru mau ke lab. Takut dihukum dosen, nanti nasib saya tragis kayak drakor makjang!" ucapnya cepat. Tanpa menunggu balasan Irfan, ia langsung berlari kecil meninggalkan koridor.
Irfan menggelengkan kepala, heran dengan tingkah cewek aneh tersebut. Namun, pandangannya tiba-tiba terikat pada selembar kertas pembatas buku yang terjatuh di dekat kakinya. Kertas itu berbahan tebal dengan kaligrafi huruf "*Z" yang digambar sangat indah di tengahnya. Di balik kertas itu, ada tulisan tangan yang rapi: *“Mulailah segala sesuatu dengan basmalah, agar berkah. Jangan lupa lanjut nonton episode 10 malam ini!”
Irfan memandangi huruf *Z* tersebut, lalu menatap koridor kosong tempat cewek berkacamata tadi menghilang. Sudut bibirnya tiba-tiba terangkat, membentuk senyuman tipis. Karakter cewek ini seolah menjadi bug baru yang siap mengacaukan sistem hidupnya yang lurus dan dingin.
"Z...?" gumam Irfan pelan, lalu memasukkan kertas itu ke dalam saku jaketnya.
Dua minggu berlalu, dan pembatas buku itu masih setia berada di saku jaket denim Irfan. Berkat instingnya sebagai anak Sistem Informasi, tidak sulit bagi Irfan untuk melacak identitas cewek itu. Nama lengkapnya Zoya, mahasiswi tingkat satu yang terkenal aktif di kegiatan kerohanian kampus, tapi sekaligus terkenal di kalangan temannya sebagai "kamus drakor berjalan".
Sore itu, hujan deras mengguyur kampus. Irfan melihat Zoya sedang berdiri di teras gedung fakultas, menatap rintik hujan dengan cemas. Tangannya memeluk tas laptop erat-erat.
Irfan berjalan mendekat, sengaja menghentikan langkah tepat di samping Zoya. "Kenapa? Nungguin aktor drakor jemput pakai payung?" sindir Irfan pelan.
Zoya menoleh kaget, mengenali cowok urakan yang ditabraknya dua minggu lalu. "Ih, Kakak yang waktu itu!" serunya, lalu cemberut. "Bukan gitu. Ini laptop saya isinya tugas semua. Saya takut kehujanan meskipun di dalam tas. Mana saya lupa bawa payung lagi. Kalau di drakor Goblin, biasanya kalau mendung gini ada yang datang payungin..."
Irfan mendengkus geli. Tanpa banyak bicara, ia membuka jaket denimnya, lalu menyampirkan jaket tebal itu di atas kepala Zoya, membungkus tubuh mungil cewek itu sekaligus tas laptopnya.
"Kak-Kakak ngapain?" tanya Zoya gagap, wajahnya mendadak memerah di balik kacamata bulatnya.
"Gak usah banyak drama. Pakai jaket gue buat ngelindungin laptop lo. Ayo, gue anter ke halte," kata Irfan datar.
Sepanjang jalan menembus hujan gerimis menuju halte, Zoya tidak berhenti mengoceh pelan, "Ini mirip adegan rebel boy yang tolongin ceweknya, tapi versi lokal... Astagfirullah, Zoya, jaga pandangan, jangan baper!" gumamnya yang masih bisa didengar jelas oleh Irfan. Irfan hanya bisa menahan tawa, merasa sistem imun di hatinya mulai jebol menghadapi kepolosan cewek di sampingnya ini.
Begitu sampai di halte yang cukup teduh, Zoya mengembalikan jaket tersebut. "Makasih banyak ya, Kak... eh, nama Kakak siapa?"
Irfan menerima jaketnya yang sedikit basah. Sebelum menjawab, ia merogoh saku dalam jaketnya dan mengeluarkan selembar pembatas buku kaligrafi yang ia simpan rapi. "Gue Irfan. Dan ini... punya lo, kan? Huruf Z."
Mata Zoya membelalak. "Wah! Pembatas buku saya yang hilang! Kok bisa di Kak Irfan?"
"Karena lo jatuhin pas nabrak gue," Irfan mendekatkan wajahnya sedikit, membuat Zoya refleks mundur satu langkah dengan gugup. "Dan gara-gara huruf Z ini, algoritma hidup gue yang tadinya berantakan, sekarang cuma fokus ke satu user. Yaitu lo."
Zoya mengerjipkan matanya beberapa kali, mencerna istilah anak IT yang dikombinasikan dengan gaya ala cowok-cowok drakor. Detik berikutnya, wajah Zoya benar-benar matang seperti tomat. Ia menutupi wajahnya dengan pembatas buku tersebut, membuat Irfan tidak bisa lagi menahan tawa lepasnya.
"Kak Irfan ih! Belajar gombal dari drakor apa, sih?!" seru Zoya malu-malu dari balik kertas.
Di bawah rintik hujan sore itu, Irfan tahu bahwa si anak motor yang liar kini telah menemukan rute pemberhentian terakhirnya. Bukan di jalanan balap yang gelap, melainkan di dunia penuh warna milik cewek agamis pencinta drakor berinisial Z.
)");

}
// Forward declaration: dipakai oleh menuHistory & menuFavorit,
// definisi lengkapnya ada di section "Baca Cerpen".
void cerpenInfo(int indexAkun, Cerpen *c);

// ─── History ─────────────────────────────────────────────────

void tambahHistory(int indexAkun, const string &judul)
{
  akun &user = daftarAkun[indexAkun];

  // Hapus entry lama kalau judul yang sama sudah pernah dibuka,
  // supaya cerpen yang dibuka lagi pindah ke paling atas (bukan duplikat).
  history *cur = user.riwayat;
  history *prev = nullptr;
  while (cur != nullptr)
  {
    if (cur->judul == judul)
    {
      if (prev == nullptr)
        user.riwayat = cur->next;
      else
        prev->next = cur->next;
      delete cur;
      user.jumlahRiwayat--;
      break;
    }
    prev = cur;
    cur = cur->next;
  }

  history *baru = new history{judul, getCurrentTimestamp(), user.riwayat, user.jumlahRiwayat + 1};
  user.riwayat = baru;
  user.jumlahRiwayat++;
}

void menuHistory(int indexAkun)
{
  while (true)
  {
    clearScreen();
    tampilHeader("HISTORY");

    akun &user = daftarAkun[indexAkun];
    if (user.riwayat == nullptr)
    {
      cout << COLOR_GRAY << "\n  Belum ada riwayat cerpen yang dibuka.\n"
           << COLOR_RESET;
      cout << "\n  Tekan Enter untuk kembali...";
      cin.ignore();
      cin.get();
      return;
    }

    int no = 1;
    history *cur = user.riwayat;
    while (cur != nullptr)
    {
      cout << COLOR_YELLOW << "  [" << no << "] " << COLOR_RESET
           << cur->judul << COLOR_GRAY << "  (" << cur->waktu << ")\n"
           << COLOR_RESET;
      no++;
      cur = cur->next;
    }

    cout << COLOR_RED << "\n  [0] Kembali\n"
         << COLOR_RESET
         << "\n  Buka cerpen no: ";

    int pilihan;
    if (!(cin >> pilihan))
    {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }

    if (pilihan == 0)
      return;

    if (pilihan < 1 || pilihan > user.jumlahRiwayat)
      continue;

    cur = user.riwayat;
    for (int i = 1; i < pilihan; i++)
      cur = cur->next;

    Cerpen *c = cariCerpenByJudul(cur->judul);
    if (c != nullptr)
      cerpenInfo(indexAkun, c);
  }
}

// ─── Menu Utama Setelah Login ─────────────────────────────────

void menuUtama(int indexAkun)
{
  clearScreen();
  tampilHeader("MENU UTAMA");
  cout << COLOR_GREEN << "\n  Halo, " << daftarAkun[indexAkun].nama << "! Selamat datang di CerpenKita. Hari ini mau ngapain?\n"
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
    tampilHeader("CERPENKITA");

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
