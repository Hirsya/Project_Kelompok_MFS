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

string buatSinopsis(const string &isi, int maxLen = 100)
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

void tambahCerpen(int id, string judul, string penulis, string genre, string isi, string waktu)
{
  Cerpen *baru = new Cerpen{id, judul, penulis, genre, isi, waktu, nullptr, nullptr};

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
  tambahCerpen(1, "Tuan Hujan", "Intan", "Drama",
               R"(Tuan Hujan
Oleh:Anak Agung Ayu Intan PM

Detik demi detik meluruh, menyeret malam melampaui batas tengahnya yang paling sepi. Dalam kungkungan kamar yang dingin, Ana memilih abadi dalam terjaga. Ruangan itu hanya diterangi sisa pendar lampu kupu-kupu yang muram di dalam kaca, ditemani luruhnya warna dari sebuket mawar biru artifisial di atas meja kayu. Keduanya meluap menjadi monumen ingatan yang menolak runtuh dari benak Nara. Dingin rintik hujan di balik jendela malam ini rupanya gagal meredam suara gemuruh yang kian bising menghantam dinding hatinya. tangannya meraih sebuah buku menyapukan pena di atas bentangan kertas bergaris, mengalirkan sebait simfoni sunyi—sebuah rasa yang barangkali, tak akan pernah sampai pada telinga sang pemilik nama.
Rintik hujan jatuh ke bumi
Suara desir angin bagai sebuah simfoni
Di langit kelabu terbentang sebuah janji
 Ia aku masih tetap disini
Ana meletakkan penanya sejenak, menatap ke luar jendela kaca tempat sisa-sisa air langit masih bergulir pelan. Baginya “Tuan Hujan”. Sebuah enigma yang kerap meninggalkan tanda tanya. Pertemuan-pertemuan mereka seumpama ketidaksengajaan yang ditulis dengan tinta takdir yang terlampau pekat.Setiap kali jemari awan meredupkan cakrawala dan ranjang langit merunduk kelabu, Ana tahu bahwa hujan akan kembali, iya kali ini kuharap ia benar-benar akan Kembali. Ana selalu luluh, terpaku menolak beranjak demi menjaga sebait janji tanpa aksara yang ia jahit sendiri di dalam dadanya. Namun, menyerahkan hati pada sosok yang bernapas dengan tabiat hujan berarti bertaruh pada ketidakpastian. Ana tahu, ada masanya di mana rintik paling karib paling hangat paling tenang itu mendadak menjelma badai, bergemuruh hebat memporak-porandakan hidup tanpa sempat memberi tanda peringatan.
Rintik hujan mendatangkan banjir
yang tak terbendung di ujung bibir
seperti sebuah petir
Begitu pula halnya hadir
Benak Ana kembali memutar sebuah untaian ingatan tentang kali pertama sepasang matanya menangkap siluet hujan. Gerimis kala itu meluruh tipis, seolah menjelma jemari tak kasat mata yang menuntun langkah kakinya menemui takdir yang kini mengurungnya dalam sepi. Hujan itu unik, bisik batin Nara waktu itu. Ia adalah seniman yang teramat misterius. Terkadang hadirnya melukiskan lengkung pelangi—sebuah simbol harapan dan keceriaan yang memeluk langit. Namun di lain hari, ia bisa berubah menjadi amukan badai hebat yang meremukkan bumi. Sering kali pula, ia hanya bertamu sesaat, sekadar untuk menyapa dan mengisyaratkan bahwa hadirnya itu nyata, sebelum akhirnya lenyap tanpa pamit.Nara menyeka tetesan hangat yang meluncur di pipinya. Tepat saat jemarinya menyentuh kulit yang basah, sekelebat cahaya membelah malam, disusul suara gemuruh yang menggelegar dahsyat, menyentak Nara dari lamunan panjangnya. Hujan selalu saja menyembunyikan rahasia di balik jubahnya.Kehadirannya, tujuannya, alasannya, dan maknanya adalah labirin teka-teki yang memaksa Nara untuk selalu menerka-nerka arti di balik rintiknya setiap saat. Sebuah pertemuan yang awalnya terasa paling hangat, yang pernah memeluk jiwanya dengan ketenangan, perlahan-lahan menguap dan lenyap. Pertemuan itu persis seperti kilatan petir malam ini—datang menyentak sekejap, menerangi dunianya dalam satu kedipan mata, lalu menyisakan kegelapan yang jauh lebih pekat. Kehadiran Tuan Hujan yang meruntuhkan seluruh pertahanan egonya, kini benar-benar menjadi banjir air mata yang tak terbendung, mengalir sunyi hingga tertahan kelu di ujung bibirnya.
Aku tak membenci hujan
walau cahaya hanya bias
ku harap genangnya membekas
Hadirmu mengajarkanku ikhlas
Nara menarik napasnya dalam-dalam, membiarkan aroma tanah basah dan dinginnya angin malam memenuhi rongga dadanya yang sesak. Ia sama sekali tidak pernah mengutuk takdir, pun ia tak pernah membenci kedatangan hujan. Meskipun Nara tahu, mencintai hujan berarti harus siap berjalan dalam keremangan, di mana cahaya kepastian tidak pernah bersinar terang, melainkan hanya berupa bias-bias redup yang memantul bak metafora dari sebuah harap. Hubungan ini adalah sebuah ketidakpastian yang nyata. Namun,ia memilih untuk berdamai dengan rasa itu. Nara kini mengerti bahwa manusia yang datang ke dalam hidup kita tidak selalu dikirim Tuhan untuk menetap hingga akhir. Ada jiwa-jiwa yang dihadirkan seumpama rintik hujan malam ini—mereka datang bukan untuk dimiliki, melainkan hanya singgah sesaat untuk meberi tahu dirimu sebuah makna, lalu menguap kembali ke langit kelabu. Ana tersenyum 
"Bukan Begitu Tuan Hujan ?...."

)", "26-08-2016");

  tambahCerpen(2, "Aroma Manis dari Dapur Nara", "Mira", "slice of life",
               R"(Pagi itu rumah Nara terasa sunyi, sunyi yang berbeda dari biasaya. Angin yang masuk lewat jendela
 ruang tamu menggerakkan tirai perlahan, seperti seseorang yang menguap panjang karena bosan.  Ayah dan ibunya sudah berangkat sejak pagi untuk menghadiri sebuah acara. Mereka meninggalkan secarik pesan manis di meja makan: “Nara yang manis, jaga rumah baik-baik ya!” Nara tersenyum kecil meski tak ada yang melihat. Walaupun ia mencoba berani, hatinya tetap saja merasa sedikit takut dan kesepian. Jam dinding masih menunjukkan pukul sembilan pagi, dan waktu sekolah enggan berjalan lebih cepat. Tiba-tiba, “ting-tong!” suara bel memecah keheningan. Nara terlonjak kecil lalu berlari menuju pintu, berharap itu bukan tamu tak dikenal. Begitu pintu dibuka, terdengar suara ceria:
“Naraaaa!! Aku dititip di sini!” Reno sepupunya yang seumuran berdiri dengan tas kecil, rambut acak-acakan, dan senyum selebar pintu. Ia melangkah masuk seperti badai kecil penuh energi, langsung menghapus seluruh sepi di rumah itu. “Yeayy! Aku nggak sendirian!” Nara menepuk tangan bahagia.Tak butuh waktu lama sampai ruang tamu berubah menjadi tempat bermain. Komik berserakan, pensil warna tumpah seperti pelangi, dan kertas gambar menumpuk di sofa. “Reno, coba gambar wajahku!” tantang Nara. Reno menggambar cepat, dan hasilnya adalah wajah bulat seperti bola dengan rambut seperti sapu ijuk. Nara menatap hasilnya. “Ini aku??” 
“Hehehe… versi kartun spesial!” jawab Reno bangga. Mereka lalu membuat komik mini tentang dua anak pemberani. Reno menirukan suara monster, membuat Nara tertawa terbahak hingga terjatuh pelan ke bantal sofa. Rumah yang tadinya hening kini terasa hidup hangat, riuh, penuh cerita. Namun di tengah tawa mereka, “Krruuukkk…”
Suara itu keras. Sangat keras. Reno memegang perutnya dengan wajah dramatis seperti tokoh film yang kehabisan dialog.
“Nar aku lapar sekali” Nara langsung berdiri. “Eh! Kita cek dulu tudung saji! Siapa tahu ada makanan!”
Mereka berjalan ke meja makan seperti dua detektif yang hendak memecahkan misteri besar. Nara memegang pinggir tudung saji, Reno menahan napas sampai pipinya menggembung. 
 “Siap, Ren?”
“Siap, komandan!”
Mereka mengangkat tudung saji itu perlahan.., tenyata Isinya hanya sepiring pisang goreng sisa semalam yang sudah dingin dan tampak menyedihkan. Reno mendekatkannya ke hidung. “Hmmm… aroma masa lalu,” katanya lirih, penuh drama. Nara tertawa. “Kamu lucu banget sih! Ya udah, kita cari makan yang beneran aja. Yang baru. Yang hangat.”  Reno mengangguk cepat seperti burung berkicau.
“Setujuuu!”
Mereka bergegas ke dapur. Nara membuka kulkas dan mengeluarkan semua bahan yang tersedia: tepung, telur, susu, mentega, madu, dan gula. Semuanya berjejer rapi seperti pasukan penyelamat. Reno mengamati bahan-bahan itu dengan mata berbinar. “Dengan ini… kita bisa bikin sesuatu yang lembut, manis, dan bikin bahagia!” Nara mengangguk mantap. “Kita bikin pancake!” Reno langsung bersorak pelan. “Pancake adalah makanan pahlawan!” Mereka mencampur bahan-bahan dengan penuh semangat.“Reno, susu sedikit saja, ya,” pesan Nara.
      Tapi Reno menuang susu seperti menuangkan hujan deras dari awan. Adonan langsung menjadi super cair. Saat dituangkan ke wajan, bentuknya melebar liar seperti benua baru yang belum    dinamai. Reno menatapnya sedih. “Nar… itu pancake atau peta dunia?” Nara tak bisa menahan    tawa. Tak ingin gagal lagi, Nara menambahkan tepung. Satu sendok. Dua sendok. Tiga sendok. Sampai adonannya setebal lumpur sawah. Saat dijatuhkan ke wajan, bunyinya cuma: “Pluk!”
Pancake itu diam. Tidak mengembang. Tidak bergerak. Seperti sedang mogok kerja. Reno mengetuknya pelan .“Ini… bukan pancake. Ini batu bata versi empuk.” Mereka berdua tertawa sampai memegangi perut. Setelah mengukur dengan teliti, adonan mereka akhirnya tampak sempurna. “Reno, api kecil ya. Biar matang merata,” kata Nara seperti chef profesional. Reno mengangguk… tapi menaikkan api sedikit karena tidak sabar menunggu hidangan manis itu matang. Tak sampai satu menit, aroma hangus menyeruak memenuhi dapur. “Renoooo!!” Nara cepat-cepat membuka jendela. “Dapur kita jadi bau arang!!” Reno menunduk menyesal. Ia membuka toples tepung,  kosong.
“Nar…, apa ini akhir dari segalanya?” katanya dengan suara kecil. Nara menjentikkan jarinya.
“Tidak! Ini saatnya kita melakukan misi baru. Kita beli tepung sekarang!”
Nara mengambil sepeda kayuhnya. Reno duduk di belakang sambil memegang kantong belanja kosong. Angin sore menerpa wajah mereka saat mereka melaju. “Semoga tepungnya masih ada ya!” Mereka membeli tepung paling besar yang tersedia dan pulang sambil tertawa karena kantong belanjanya lebih besar dari perut Reno. Dapur kini terasa lebih tenang. Cahaya matahari sore masuk lembut, membuat meja dapur tampak berkilau hangat.
Reno menuang susu perlahan—sangat hati-hati. Nara mengaduk adonan sampai teksturnya halus seperti awan manis. Ketika dituangkan ke wajan, terdengar suara lembut: 
“SssShh…” Gelembung kecil muncul. Aroma vanila menyebar, memenuhi dapur seperti pelukan hangat yang memanggil kenangan. Nara membalik pancake itu. Plup! Warnanya sempurna: kecokelatan lembut seperti roti panggang pagi hari. Reno ternganga kagum. “Nar… ini pancake sungguhan! Mereka menuangi madu yang jatuh perlahan, berkilau seperti tetesan sinar matahari cair. “Yuk kita coba!” kata Nara. Gigitan pertama terasa lembut, hangat, dan manis. Semua kegagalan tadi terasa lucu dan berharga. Reno tersenyum lebar. “Nar… aroma manis dari dapurmu bikin hati ikut bahagia.”
Nara ikut tersenyum. Hari yang tadinya sepi kini berubah menjadi hari penuh tawa, kehangatan, dan aroma manis yang mengajarkan bahwa kebahagiaan sering lahir dari kegagalan-kegagalan kecil. 
  )", "30-11-2018");

  tambahCerpen(3, "Misteri Rumah Tua", "Maya", "Horor",
               "Rumah tua di ujung desa itu sudah lama ditinggalkan penghuninya. Konon, setiap malam Jumat Kliwon, terdengar suara tangisan dari dalam rumah tersebut. Malam itu, empat orang remaja memutuskan untuk membuktikan kebenarannya, tanpa tahu apa yang menanti mereka di dalam.", "26-12-2019");

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
menjadi kenyataan mengerikan yang menghancurkan segalanya.)", "21-11-2020");

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
)", "05-06-2021");

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

// ─── Bookmark / Favorit ──────────────────────────────────────

Bookmark *cariBookmark(int indexAkun, const string &judul)
{
  Bookmark *cur = daftarAkun[indexAkun].bookmark;
  while (cur != nullptr)
  {
    if (cur->judul == judul)
      return cur;
    cur = cur->next;
  }
  return nullptr;
}

void tambahBookmark(int indexAkun, const string &judul)
{
  akun &user = daftarAkun[indexAkun];
  Bookmark *baru = new Bookmark{judul, user.bookmark, user.jumlahBookmark + 1};
  user.bookmark = baru;
  user.jumlahBookmark++;
}

void hapusBookmark(int indexAkun, const string &judul)
{
  akun &user = daftarAkun[indexAkun];
  Bookmark *cur = user.bookmark;
  Bookmark *prev = nullptr;

  while (cur != nullptr)
  {
    if (cur->judul == judul)
    {
      if (prev == nullptr)
        user.bookmark = cur->next;
      else
        prev->next = cur->next;
      delete cur;
      user.jumlahBookmark--;
      return;
    }
    prev = cur;
    cur = cur->next;
  }
}

void toggleBookmark(int indexAkun, const string &judul)
{
  if (cariBookmark(indexAkun, judul) != nullptr)
  {
    hapusBookmark(indexAkun, judul);
    cout << COLOR_RED << "\n  Dihapus dari favorit.\n"
         << COLOR_RESET;
  }
  else
  {
    tambahBookmark(indexAkun, judul);
    cout << COLOR_GREEN << "\n  Ditambahkan ke favorit.\n"
         << COLOR_RESET;
  }
  cout << "  Tekan Enter...";
  cin.ignore();
  cin.get();
}

void menuFavorit(int indexAkun)
{
  while (true)
  {
    clearScreen();
    tampilHeader("FAVORIT");

    akun &user = daftarAkun[indexAkun];
    if (user.bookmark == nullptr)
    {
      cout << COLOR_GRAY << "\n  Belum ada cerpen favorit.\n"
           << COLOR_RESET;
      cout << "\n  Tekan Enter untuk kembali...";
      cin.ignore();
      cin.get();
      return;
    }

    int no = 1;
    Bookmark *cur = user.bookmark;
    while (cur != nullptr)
    {
      cout << COLOR_YELLOW << "  [" << no << "] " << COLOR_RESET << cur->judul;

      Cerpen *c = cariCerpenByJudul(cur->judul);
      if (c != nullptr)
        cout << COLOR_GRAY << " - " << c->penulis << " (" << c->genre << ")" << COLOR_RESET;

      cout << "\n";
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

    if (pilihan < 1 || pilihan > user.jumlahBookmark)
      continue;

    cur = user.bookmark;
    for (int i = 1; i < pilihan; i++)
      cur = cur->next;

    Cerpen *c = cariCerpenByJudul(cur->judul);
    if (c != nullptr)
      cerpenInfo(indexAkun, c);
  }
}

// ─── Baca Cerpen ─────────────────────────────────────────────

void tampilSemuaCerpen()
{
  if (headCerpen == nullptr)
  {
    cout << COLOR_GRAY << "\n  Belum ada cerpen.\n"
         << COLOR_RESET;
    return;
  }

  Cerpen *cur = headCerpen;
  while (cur != nullptr)
  {
    cout << COLOR_YELLOW << "  [" << cur->id << "] " << COLOR_RESET
         << cur->judul << "\n";
    cur = cur->next;
  }
}
void tambahKomentar(Cerpen *c, int rating, const string &nama, const string &isi)
{
  Komentar *baru = new Komentar{rating, nama, isi, getCurrentTimestamp(), c->komentarHead};
  c->komentarHead = baru;
}

void menuRatingKomentar(int indexAkun, Cerpen *c)
{
  clearScreen();
  tampilHeader("RATING & KOMENTAR");
  cout << COLOR_WHITE << "\n  Cerpen : " << COLOR_RESET << c->judul << "\n";

  int rating = 0;
  while (true)
  {
    cout << "\n  Beri rating (1-5) : ";
    if (!(cin >> rating))
    {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << COLOR_RED << "  Input tidak valid!\n" << COLOR_RESET;
      continue;
    }
    if (rating < 1 || rating > 5)
    {
      cout << COLOR_RED << "  Rating harus antara 1 dan 5.\n" << COLOR_RESET;
      continue;
    }
    break;
  }

  string isiKomentar;
  cout << "  Tulis komentar    : ";
  cin.ignore();
  getline(cin, isiKomentar);

  tambahKomentar(c, rating, daftarAkun[indexAkun].nama, isiKomentar);

  cout << COLOR_GREEN << "\n  Rating dan komentar berhasil disimpan!\n" << COLOR_RESET;
  cout << "  Tekan Enter...";
  cin.get();
}

void tampilKomentar(int indexAkun, Cerpen *c)
{
  clearScreen();
  tampilHeader("KOMENTAR");
  cout << COLOR_WHITE << "\n  Cerpen : " << COLOR_RESET << c->judul << "\n";

  if (c->komentarHead == nullptr)
  {
    cout << COLOR_GRAY << "\n  Belum ada komentar untuk cerpen ini.\n"
         << COLOR_RESET;
  }
  else
  {
    cout << "\n";
    Komentar *cur = c->komentarHead;
    while (cur != nullptr)
    {
      cout << COLOR_YELLOW << "  " << cur->nama << COLOR_RESET
           << " [" << cur->rating << "/5]"
           << COLOR_GRAY << "  " << cur->waktu << "\n"
           << COLOR_RESET
           << "  " << cur->isi << "\n\n";
      cur = cur->next;
    }
  }

  cout << COLOR_YELLOW << "  [1] Beri Rating & Komentar\n"
       << COLOR_RED << "  [0] Kembali\n"
       << COLOR_RESET
       << "\n  Pilih: ";

  int pilihan;
  if (!(cin >> pilihan))
  {
    cin.clear();
    cin.ignore(1000, '\n');
    return;
  }

  if (pilihan == 1)
    menuRatingKomentar(indexAkun, c);
}

void tampilLayarBuka(Cerpen *c)
{
  clearScreen();
  cout << "\n\n";

  const int INNER = 38;
  cout << COLOR_CYAN << "|======================================|\n"
       << "|                                      |\n";

  // Judul di tengah
  cout << "|" << COLOR_WHITE;
  string judulTampil = c->judul;
  if ((int)judulTampil.size() > INNER) judulTampil = judulTampil.substr(0, INNER - 3) + "...";
  int padJ = (INNER - (int)judulTampil.size()) / 2;
  for (int i = 0; i < padJ; i++) cout << ' ';
  cout << judulTampil;
  for (int i = 0; i < INNER - padJ - (int)judulTampil.size(); i++) cout << ' ';
  cout << COLOR_CYAN << "|\n"
       << "|                                      |\n";

  // Penulis di tengah
  cout << "|" << COLOR_GRAY;
  string penulisBaris = "~ " + c->penulis + " ~";
  if ((int)penulisBaris.size() > INNER) penulisBaris = penulisBaris.substr(0, INNER);
  int padP = (INNER - (int)penulisBaris.size()) / 2;
  for (int i = 0; i < padP; i++) cout << ' ';
  cout << penulisBaris;
  for (int i = 0; i < INNER - padP - (int)penulisBaris.size(); i++) cout << ' ';
  cout << COLOR_CYAN << "|\n"
       << "|                                      |\n"
       << "|======================================|\n"
       << COLOR_RESET;

  cout << "\n  " << COLOR_GRAY << "Tekan Enter untuk mulai membaca..."
       << COLOR_RESET;
  cin.ignore(1000, '\n');
  cin.get();
}

vector<string> pecahBaris(const string &teks, int lebarMaks)
{
  vector<string> hasil;
  string barisSaat;
  string src = normalkanKarakter(teks);

  for (size_t i = 0; i <= src.size(); i++) {
    char ch = (i < src.size()) ? src[i] : '\n';

    if (ch == '\n') {
      while ((int)barisSaat.size() > lebarMaks) {
        int cut = lebarMaks;
        while (cut > 0 && barisSaat[cut] != ' ')
          cut--;
        if (cut == 0) cut = lebarMaks;
        hasil.push_back(barisSaat.substr(0, cut));
        barisSaat = barisSaat.substr(
          (cut < (int)barisSaat.size() && barisSaat[cut] == ' ') ? cut + 1 : cut
        );
      }
      hasil.push_back(barisSaat);
      barisSaat.clear();
    } else {
      barisSaat += ch;
    }
  }

  return hasil;
}

void tampilIsiCerpen(int indexAkun, Cerpen *c)
{
  tampilLayarBuka(c);

  const int BARIS_PER_HALAMAN = 20;
  vector<string> baris = pecahBaris(c->isi, 68);
  int totalHalaman = ((int)baris.size() + BARIS_PER_HALAMAN - 1) / BARIS_PER_HALAMAN;
  if (totalHalaman == 0) totalHalaman = 1;
  int halSaat = 1;

  while (true) {
    clearScreen();
    tampilHeader(c->judul);
    cout << COLOR_GRAY << "  Halaman " << halSaat << " / " << totalHalaman
         << COLOR_RESET << "\n\n";

    int mulai = (halSaat - 1) * BARIS_PER_HALAMAN;
    int akhir = mulai + BARIS_PER_HALAMAN;
    if (akhir > (int)baris.size()) akhir = (int)baris.size();

    for (int i = mulai; i < akhir; i++)
      cout << "  " << baris[i] << "\n";

    cout << COLOR_CYAN << "\n  ------------------------------------------\n"
         << COLOR_RESET;

    if (halSaat < totalHalaman)
      cout << COLOR_YELLOW << "  [N] Halaman Berikutnya\n" << COLOR_RESET;
    if (halSaat > 1)
      cout << COLOR_YELLOW << "  [P] Halaman Sebelumnya\n" << COLOR_RESET;
    cout << COLOR_GREEN << "  [K] Komentar\n"
         << COLOR_RED   << "  [0] Selesai Baca\n"
         << COLOR_RESET << "\n  Pilih: ";

    char ch;
    if (!(cin >> ch)) {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }
    ch = toupper((unsigned char)ch);

    if      (ch == 'N' && halSaat < totalHalaman) halSaat++;
    else if (ch == 'P' && halSaat > 1)            halSaat--;
    else if (ch == 'K')                            tampilKomentar(indexAkun, c);
    else if (ch == '0')                            return;
  }
}

void cerpenInfo(int indexAkun, Cerpen *c)
{
  while (true)
  {
    clearScreen();
    tampilHeader(c->judul);

    cout << COLOR_WHITE << "\n  Penulis      : " << COLOR_RESET << c->penulis << "\n"
         << COLOR_WHITE << "  Genre        : " << COLOR_RESET << c->genre << "\n"
         << COLOR_WHITE << "  Ditambahkan  : " << COLOR_RESET << c->waktu << "\n";

    double rating = hitungRatingRata(c);
    cout << COLOR_WHITE << "  Rating  : " << COLOR_RESET;
    if (rating <= 0)
      cout << COLOR_GRAY << "Belum ada rating" << COLOR_RESET << "\n";
    else
      cout << rating << " / 5\n";

    cout << COLOR_WHITE << "\n  Sinopsis:\n"
         << COLOR_RESET << "  " << buatSinopsis(c->isi) << "\n";

    bool favorit = cariBookmark(indexAkun, c->judul) != nullptr;
    cout << "\n  [1] Baca Cerpen\n";
    if (favorit)
      cout << COLOR_RED << "  [2] Hapus dari Favorit\n"
           << COLOR_RESET;
    else
      cout << COLOR_GREEN << "  [2] Tambah ke Favorit\n"
           << COLOR_RESET;
    cout << COLOR_RED << "  [0] Kembali\n"
         << COLOR_RESET
         << "\n  Pilih: ";

    int pilihan;
    if (!(cin >> pilihan))
    {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }

    switch (pilihan)
    {
    case 1:
      tambahHistory(indexAkun, c->judul);
      tampilIsiCerpen(indexAkun, c);
      break;
    case 2:
      toggleBookmark(indexAkun, c->judul);
      break;
    case 0:
      return;
    }
  }
}

void menuListCerpen(int indexAkun)
{
  while (true)
  {
    clearScreen();
    tampilHeader("DAFTAR CERPEN");
    cout << "\n";
    tampilSemuaCerpen();

    cout << COLOR_RED << "\n  [0] Kembali\n"
         << COLOR_RESET
         << "\n  Pilih cerpen (ID): ";

    int pilihan;
    if (!(cin >> pilihan))
    {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }

    if (pilihan == 0)
      return;

    Cerpen *c = cariCerpenById(pilihan);
    if (c == nullptr)
    {
      cout << COLOR_RED << "\n  Cerpen tidak ditemukan!\n"
           << COLOR_RESET;
      cout << "  Tekan Enter...";
      cin.ignore();
      cin.get();
      continue;
    }

    cerpenInfo(indexAkun, c);
  }
}

// ─── Tambah Cerpen ───────────────────────────────────────────

void menuTambahCerpen(int indexAkun)
{
  clearScreen();
  tampilHeader("TAMBAH CERPEN");

  string judul, genre, isi, waktu = getCurrentTimestamp();

  cout << "\n  " << COLOR_GRAY << "(ketik 0 untuk batal)\n"
       << COLOR_RESET;

  cout << "\n  Judul  : ";
  cin.ignore();
  getline(cin, judul);

  if (judul == "0" || judul.empty())
    return;

  cout << "  Genre  : ";
  getline(cin, genre);

  if (genre == "0")
    return;

  cout << "  Isi    : ";
  getline(cin, isi);

  if (isi.empty())
  {
    cout << COLOR_RED << "\n  Isi cerpen tidak boleh kosong!\n"
         << COLOR_RESET;
    cout << "  Tekan Enter...";
    cin.get();
    return;
  }

  tambahCerpen(jumlahCerpen + 1, judul, daftarAkun[indexAkun].nama, genre, isi, waktu);

  cout << COLOR_GREEN << "\n  Cerpen berhasil ditambahkan!\n"
       << COLOR_RESET;
  cout << "  Tekan Enter...";
  cin.get();
}

// ─── Menu Utama Setelah Login ─────────────────────────────────

void menuUtama(int indexAkun)
{
  while (true)
  {
    clearScreen();
    tampilHeader("MENU UTAMA");
    cout << COLOR_GREEN << "\n  Halo, " << daftarAkun[indexAkun].nama << "!\n"
         << COLOR_RESET;

    cout << COLOR_YELLOW
         << "\n  [1] Lihat Semua Cerpen\n"
         << "  [2] History\n"
         << "  [3] Favorit\n"
         << "  [4] Tambah Cerpen\n"
         << COLOR_RED
         << "  [0] Logout\n"
         << COLOR_RESET
         << "\n  Pilih: ";

    int pilihan;
    if (!(cin >> pilihan))
    {
      cin.clear();
      cin.ignore(1000, '\n');
      continue;
    }

    switch (pilihan)
    {
    case 1:
      menuListCerpen(indexAkun);
      break;
    case 2:
      menuHistory(indexAkun);
      break;
    case 3:
      menuFavorit(indexAkun);
      break;
    case 4:
      menuTambahCerpen(indexAkun);
      break;
    case 0:
      return;
    default:
      cout << COLOR_RED << "\n  Pilihan tidak valid!\n"
           << COLOR_RESET;
      cout << "  Tekan Enter...";
      cin.ignore();
      cin.get();
    }
  }
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
// ─── Pembersih Memori ─────────────────────────────────────────

void bersihkanMemori() {
  // 1. Bersihkan riwayat (history) dan favorit (bookmark) di semua akun
  for (int i = 0; i < jumlahAkun; i++) {
    
    // Membersihkan Linked List History
    history *curHist = daftarAkun[i].riwayat;
    while (curHist != nullptr) {
      history *tempH = curHist;
      curHist = curHist->next;
      delete tempH;
    }
    daftarAkun[i].riwayat = nullptr; // Mengembalikan pointer ke null (Best Practice)
    daftarAkun[i].jumlahRiwayat = 0;

    // Membersihkan Linked List Bookmark
    Bookmark *curBook = daftarAkun[i].bookmark;
    while (curBook != nullptr) {
      Bookmark *tempB = curBook;
      curBook = curBook->next;
      delete tempB;
    }
    daftarAkun[i].bookmark = nullptr;
    daftarAkun[i].jumlahBookmark = 0;
  }

  // 2. Bersihkan daftar cerpen beserta semua komentar di dalamnya
  Cerpen *curCerpen = headCerpen;
  while (curCerpen != nullptr) {
    
    // Membersihkan Linked List Komentar di dalam cerpen yang sedang ditunjuk
    Komentar *curKom = curCerpen->komentarHead;
    while (curKom != nullptr) {
      Komentar *tempK = curKom;
      curKom = curKom->next;
      delete tempK;
    }
    curCerpen->komentarHead = nullptr;

    // Setelah komentar bersih, baru hapus node Cerpen itu sendiri
    Cerpen *tempC = curCerpen;
    curCerpen = curCerpen->next;
    delete tempC;
  }
  
  headCerpen = nullptr;
  jumlahCerpen = 0;
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
    	bersihkanMemori();
      clearScreen();
      cout << COLOR_GREEN << "  Sampai jumpa! Terimakasih Sudah Berkunjung :) \n\n"
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
