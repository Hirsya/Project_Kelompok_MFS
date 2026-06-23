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
#define COLOR_GRAY "\033[0;36m"
#define COLOR_WHITE "\033[1;37m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_PINK "\033[95m"

// ─── Structs ─────────────────────────────────────────────────
const int MAX_KOMENTAR = 100;
const int MAX_HISTORY = 50;
const int MAX_BOOKMARK = 50;
const int MAX_CERPEN_TOTAL = 100;

struct Komentar
{
  int rating;
  string nama;
  string isi;
  string waktu;
};

struct Cerpen
{
  int id;
  string judul;
  string penulis;
  string genre;
  string isi;
  string waktu;
  Komentar daftarKomentar[MAX_KOMENTAR];
  int jumlahKomentar;
};

struct history
{
  string judul;
  string waktu;
  int idHistory;
  int idCerpen;
  int halamanTerakhir;
};

struct Bookmark
{
  string judul;
  int idBookmark;
};

struct akun
{
  string nama;
  string password;
  history riwayat[MAX_HISTORY];
  Bookmark bookmark[MAX_BOOKMARK];
  int jumlahRiwayat;
  int jumlahBookmark;
};

// ─── Global ──────────────────────────────────────────────────

const int MAX_AKUN = 100;
akun daftarAkun[MAX_AKUN];
int jumlahAkun = 0;

Cerpen daftarCerpen[MAX_CERPEN_TOTAL];
int jumlahCerpen = 0;

// ─── Helper ──────────────────────────────────────────────────

void clearScreen()   {  cout << "\033[2J\033[3J\033[1;1H"; }
void hideKursor()    { cout << "\033[?25l" << flush; }
void showKursor()    { cout << "\033[?25h" << flush; }

int menuSelector(const vector<string> &menu, int selected)
{
    int ch = _getch();

    if (ch == 0 || ch == 224)
    {
        ch = _getch();
        if (ch == 72) selected = (selected - 1 + menu.size()) % menu.size();
        if (ch == 80) selected = (selected + 1) % menu.size();
    }
    else if (ch == 13)
    {
        return -selected - 1; 
        // pakai trik: ENTER
    }

    return selected;
}

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
  for (int i = 0; i < jumlahCerpen; i++)
  {
    if (daftarCerpen[i].id == id)
      return &daftarCerpen[i];
  }
  return nullptr;
}

Cerpen *cariCerpenByJudul(const string &judul)
{
  for (int i = 0; i < jumlahCerpen; i++)
  {
    if (daftarCerpen[i].judul == judul)
      return &daftarCerpen[i];
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
  if (c->jumlahKomentar == 0)
    return 0;

  int total = 0;
  for (int i = 0; i < c->jumlahKomentar; i++)
  {
    total += c->daftarKomentar[i].rating;
  }
  return (double)total / c->jumlahKomentar;
}

void tambahCerpen(int id, string judul, string penulis, string genre, string isi, string waktu)
{
  if (jumlahCerpen >= MAX_CERPEN_TOTAL) return;
  
  daftarCerpen[jumlahCerpen].id = id;
  daftarCerpen[jumlahCerpen].judul = judul;
  daftarCerpen[jumlahCerpen].penulis = penulis;
  daftarCerpen[jumlahCerpen].genre = genre;
  daftarCerpen[jumlahCerpen].isi = isi;
  daftarCerpen[jumlahCerpen].waktu = waktu;
  daftarCerpen[jumlahCerpen].jumlahKomentar = 0;
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

  tambahCerpen(2, "Aroma Manis dari Dapur Nara", "Mira", "Slice of Life",
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

  tambahCerpen(3, "Merindu dan Gemanya", "Nathan", "Drama",
               R"(Bunyi yang memasuki telingaku dipagi hari Minggu ini diawali dengan pesan dari Group Chat angkatanku semasa SMA. Setelah kubuka, ternyata pesan tersebut berisi file undangan reuni angkatan 2014/2015, yang membuatku menghela nafas kasar sebab aku sangat malas menghadiri acara tersebut. Bukan karena aku anti sosial tetapi, karena aku sangat malas untuk bertemu dengan teman-temanku yang hingga saat ini pun masih bertemu dengan mereka. Dan juga aku sedikit ragu untuk bertemu kembali dengan inti dari gemaku dahulu. Aku tahu dia akan menghadiri acara tersebut, sebab namanya berada disususan acara sebagai perwakilan angkatan untuk menyumbangkan suara emasnya itu. Entahlah, mungkin aku akan memikirkannya kembali untuk menghadiri acara itu atau tidak. Namun, tidak dapat dipungkiri bahwa aku rindu mengenang masa-masa SMA ku.
Kring!, Kring!, Kring!.... Dering ponselku kembali menggangguku untuk melanjutkan aktivitas di hari libur yang akan kugunakan untuk rebahan ini. Kuangkat telepon itu tanpa melihat nama siapa yang tertera disana
“Haloo...” sapa orang dipanggilan tersebut.
“Ganggu bat dah Geng, kenapa khe  telfon-telfon?” Tanyaku dengan nada tinggi pada Ageng, teman SMA ku. 
“Gabut aja, khe kangen gak sama aku?” Ageng dengan nada centilnya.
Seketika aku langsung mematikan telepon dari Ageng itu dan kembali ke posisi tidurku. Tidak lama ponselku kembali berdering dan pastinya dari Ageng lagi. 
“Sekali lagi khe gak jelas, ku block ya” ucapku disertai penekanan disetiap katanya. 
“Eitss canda candaa, cepat kali marahnya nok . Aku cuma mau nanya, khe dah baca undangan reuni tu gak?” Tanya Ageng. 
Ternyata tujuan telepon dari Ageng adalah menanyakan mengenai undangan itu, padahal dia bisa saja menanyakan lewat chat. 
“Udah, kenapa emang?” nada yang kesal tetap kugunakan
Khe pergi gak? Kalo khe pergi samaan yo, males aku pergi sendiri” pinta Ageng 
“Gak tau, aku masih pikir-pikir lagi” kataku
Kukatakan begitu karena sesuai dengan kenyataannya bahwa aku masih menimbang-nimbang untuk pergi atau tidak. 
“Lho kenapa masih pikir-pikir, pergi jak, kan samaan aku juga. Atau khe takut ketemu sama dia ya? Tadi aku baca susunan acaranya, ada nama dia disitu” Tebakan Ageng.
Ini dia yang kukagum dari temanku, ia bisa menebak apa yang kupikirkan, tetapi tentu saja akan kututupi semua itu. 
“Mana ada begitu, orang aku males aja pergi-pergi acara gituan” alibiku pada Ageng. 
“Terserah khe dah yang penting aku dah ajak, kalo khe jadi pergi kasih tau aku, biar samaan perginya.” balasnya.
“Iya-iyaa, dah aku mau lanjut rebahan” setelah kata itu keluar dari mulutku, langsung kumatikan telepon itu dan kembali melanjutkan aktivitas terpentingku, tidur.
Gita POV
“Iyaaa Widya, nanti aku nginep dirumahmu waktu di Bali” ucapan pastiku.
Pagi hariku sudah diisi dengan telepon dari Widya, temenku yang meminta agar aku menginap dirumahnya saat di Bali nanti. Semalam aku mendapat undangan Reuni SMA angkatanku, acara itu akan dilaksanakan di Bali nantinya.Yang membuatku terkejut adalah namaku tertera pada bagian pengisi acara,sehingga membuatku mau tidak mau harus datang pada acara itu. Sebenarnya bisa saja aku menolak, namun aku tidak enak dengan tim acaranya, dan juga aku sedikit kangen sih sama sekolah tercintaku itu, sekolah yang penuh dengan kenangan bersama temanku, guru, dan juga dengan gemaku dahulu. Yahh, aku akan mengenangnya nanti saja, sekarang lebih baik mempersiapkan tiket berangkat dan semacamnya
Gita POV
(Kantor Tempat Gita Bekerja)
“Hikss.. Hikss.. Hikss.. kangennn” tangisku berseru pada benda digital didepanku. 
“Eh lo kenapaa Git, sakit apa gimana?” tanya Ratih dengan pandangan yang masih pada layar dihadapannya. 
“Inii, gw ada reuni SMA jadi gw mau liat-liat album foto lama gw di hp, terus kangennn” ungkapku dengan sedih dan sedikit tersungguk-sungguk. 
“Oalahh, kupikir kenapa, kalo kangen kenapa gak hubungin mereka?” kata Ratih.
Bener juga sih kata Ratih, tapi masalahnya 
“Aku gak ada nomornya hiks…” yap, itulah masalahnya. 
“Yaelahh, gak mungkin lo gak berteman di media sosial sama mereka kan, dm aja kali, terus minta  nomornya, beres deh” perkataan Ratih tidak ada yang salah, masalahnya ada di aku. 
“Maluuu” ungkapku.
Jujur saja entah kenapa aku malu, padahal mereka adalah  teman-temanku dulu dan saat ini Ratih hanya diam.
”Kok lo gak ngomong apa-apa sihh?” kesalku.
Padahal dia yang nanya kenapa, sekarang dia yang diam. 
”Ya gw harus apa Gita, gw paling males kalo lo udh kayak gini, makanya gw diem aja” alasan Ratih. 
Aku hanya cemberut dan menerima pernyataan yang diberi Ratih. 
Aku cek kembali tiket pesawatku dan terlihat tanggal keberangkatanku pada hari Kamis, 5 September 2024. Terhitung tinggal beberapa hari lagi, aku harus segera menyiapkan izin cuti dan keperluanku, serta sudah kuputuskan sepulang kantor akan ke toko pakaian bersama Ratih untuk membeli outfit yang cocok denganku nantinya.
(Toko Pakaian)
Sesampainya di toko pakaian yang disarankan oleh Ratih.
“Bagaimana kalo kamu pilih baju di toko ini saja git, kayanya bagus deh sesuai sama gaya berpakaianmu” saran dari Ratih.
“Boleh tuh yuk masuk, aku mau liat-liat dulu” ajak Gita
Mereka mulai mencari pakaian yang sekiranya cocok buat outfit Gita untuk acara reuni nanti. Gita mulai mencoba beberapa pakaian dan tak lupa menanyakan saran ke Ratih.
“Ratih bagaimana dengan baju ini?” 
“Eum kayanya kurang cocok deh”
Karena Ratih kurang setuju dengan pilihan Gita, Ratih pun ikut membantu memilih juga.
“Gitaaa Gitaa Gitaa!” seru Ratih dengan nada hebohnya itu.
“Apasih, heboh banget tau” kesal Gita kepada Ratih 
“Coba deh kamu liat baju sama celana ini, cocok deh buat kamu”
“Pastel banget ya, tapi aku mau coba deh” 
Gita mulai mencoba beberapa pakaian yang sudah dipilihkan oleh Ratih. Tak dipungkiri Ratih memang jago dalam hal memilih outfit seperti ini, makanya Gita sangat beruntung memiliki teman seperti Ratih karenakan sangat bisa di handalkan dalam hal seperti ini. 
“Gimana, bagus ngga?” tanya Gita kepada Ratih sambil ia memutar badan untuk memperlihatkan outfit pilihan Ratih.
“Ih cantik banget, cocok tuh buat kamu” puji Ratih
“Yang bener, yaudah deh, aku mau coba lagi satu outift yang kamu pilihin” 
Gita memasuki lagi ruang ganti baju yang ada di toko itu, dan mencoba kembali satu outfit pilihan Ratih. Sementara Ratih masih mencari-cari pakaian lagi buat buat Gita.  
“Ratih Ratih Ratih!” panggil Gita heboh
“Kenapa sih” kesal Ratih kepda Gita
“Liat bagus ngga warna biru ini?” tanya Gita untuk memastikannya. 
“Bagus dong, kan aku yang pilihkan” jawab Ratih dengan sombongnya.
“Ish serius loh, ini kan sama modelnya sama baju yang pertama, cuman beda warna aja” ucap Gita.
“Serius, yang pertama kan polos, kalo ini ada garis-garisnya gitu” bantah Ratih.
“Yaudah deh, aku ambil semuanya aja deh buat jaga-jaga juga nanti”  jawab Gita
Akhirnya setelah beberapa jam memilih dan mecoba beberapa pakaian Gita menemukan pakaian yang cocok buat dirinya. Gita pun ke kasir untuk membayar beberapa pakaian yang telah ia pilih.  Setelah membayar pakaian, ia pun kembali pulang bersama Ratih.
Gita POV
(Bandara Surabaya)
Tibalah dimana hari keberangkatan Gita ke Bali. Dengan perasaan yang campur aduk ia melangkahkan kakinya menuju bandara dengan di temani Ratih. 
“Udah ngga ada yang ketinggalan kan?” tanya Ratih untuk memastikan sudah tidak ada barang yang ketinggalan.
“Aman kok Rat, sudah aku cek tadi sebelum berangkat” jawab Gita meyakin Ratih.
“Baguslah kalo gitu, hati-hati ya git disana jangan lama-lama nanti aku kangen” ucap Ratih sedih.
“Hahaha, ngga lama kok Rat cuman 3 atau 4 harian aja nanti kalo lama bagaimana sama kerjaanku yang disini” balas Gita sambil memeluk Ratih sebagai ucapan selamat tinggal.
“Ratih, bentar lagi pesawatnya mau Take Off nih. Aku ke ruang tunggu sekarang ya, terimakasih sudah nganterin aku” pamit Gita kepada Ratih.
“Safe flight ya Git, dadahh” Ratih melambaikan tangannya.
Author POV
(Bandara Bali)
Jam menunjukkan pukul 12.00 pesawat yang dinaikan oleh Gita sudah landing di kota tujuannya yaitu Bali. Kota yang penuh kenangan, dan kebahagiannya dimasa putih abu-abu kemarin. Sesampainya di Bandara, Gita di sambut oleh teman baiknya semasa SMA yaitu Widya.
“GITAAAAA!!!” teriak Widya sambil melambaikan tangannya ke Gita.
Gita pun berlari kecil ke arah seseorang yang memanggilnya, dan mereka berpelukan untuk melepas rindu yang telah lama mereka simpan.
“AAAA... aku kangen banget sama kamu Wid, kamu apa kabar?” ucap Gita saat mereka  berpelukan.
“Aku juga kangen banget sama kamu Git, kabar aku baik. Kamu sendiri gimana?” tanya Widya balik.
“Alhamdulillah, Kabar aku baik Wid” jawab Gita.
Diperjalan pulang ke rumah Widya mereka berbincang-bincang membahas kenangan mereka saat masa SMA. Saking asiknya membahas kenangan tak terasa sudah mereka telah tiba di depan bangunan yang begitu indah, dengan ukiran-ukiran khas Balinya. 
“Wahh, di Bali udaranya seger yaa dan ukirannya juga sangat berseni sekali.”
Gita kagum melihat ukiran bali dan bangunannya yang telah lama tidak dilihat.
“Jelas dong… Bikin kangen kannn” ucap Widya dengan bangga.
“Iya nih” Gita tersenyum senang.
Mereka pun akhirnya masuk ke rumah yang berukiran indah itu yang merupakan rumah dari Widya.  Di dalam, mereka di sambut hangat oleh ibu dari Widya dan dipersilahkan untuk masuk.
”Halo Gita, lama ga keliatan. Gimana kabarnya Gita?” Ibu Widya menyambut dan menanyakan kabar dengan ramah.
“Alhamdulillah baik tante, tante gimana kabarnya?”
“Kabar tante baik. Kamu pasti capek kan diperjalanan tadi, istirahat gih di kamarnya Widya”
“Baik tante, Gita izin ke kamar dulu ya tan” pamit Gita kepada ibu Widya.
Gita dan Widya pun masuk ke dalam kamar yang sangat menggambarkan Widya perempuan yang sederhana. Didalam kamar Gita langsung merebahkan badannya ke kasur empuk milik Widya, tak lama ia pun terlelap.
Gita POV
(H-1 Acara Reuni)
“Widya ayo cepat lagi bentar kita telat nih” panggil Gita saat ia melihat arlojinya yang menunjukkan pukul 18.30.
“Iya sebentar Git, lagi nyari sepatu dulu” balas Widya.
Gita dan Widya akan berangkat ke sekolah mereka yang dulu yaitu SMA Aksara Pramana. Mereka akan melakukan gladi bersih terlebih dahulu untuk persiapan reuni mereka di keesokan harinya.  Sesampainya di sekolah, mereka langsung menuju aula yang dimana nantinya akan menjadi tempat mereka melepas rindu. 
Gita dan teman-teman lainnya yang menjadi pengisi acara reuni nantinya mulai melakukan gladi bersih. Mulai dari acara pembuka, acara inti, hingga acara penutup. Gita membawakan beberapa lagu favoritnya untuk acara tersebut. Tak terasa jam sudah menunjukkan pukul 20.30, Gita dan teman-temannya yang lain mengakhiri kegiatan gladi bersihnya.
Chandra POV
“Chandraa woyyy Chandraa!!” 
“Apa si Geng teriak-teriak???” Chandra menutup kupingnya mendengar suara nyaring Ageng.
“Yee khe lupa apa gimana si, kan nanti malam acara reuni kita nih” Ujar Ageng dengan alis yang menyatu.
“Ya terus kenapa, khe ngapain cariin aku”
“Temenin aku pergi yok cari pakaian buat acaranya”
“Ngga ah, males kali aku nok”
“Ih bentaran aja, aku ngga punya baju loh buat acara nanti malam nih”
“Ishh iya iya, awas aja lama”
“iyaa Chandraa”
Ageng dan Chandra akhirnya pergi mencari pakaian untuk acara reuni mereka nanti malam. Meski Chandra awalnya menolak, tapi pada akhirnya Chandra menyetujuinya. Ia pikir ia juga harus mencari pakaian untuk acara nanti malam. Namun, selama perjalanan ke toko baju itu, Chandra diam tak banyak bicara entah apa yang ia pikirkan, akankah hadir sosok yang selama ini ia tunggu-tunggu, sosok yang berhasil membutanya merindu selama ini.
“Kenapa sih khe melamun dari tadi” tanya Ageng untuk membuyarkan lamunan Chandra
“Hah?, mana ada aku melamun. Eh udah sampai ya?” Alibi Chandra.
“hm” Balas Ageng.
Tak ada pembicaraan lagi diantara mereka, mereka pun turun dari mobil dan berjalan masuk ke dalam toko yang menjadi tujuan mereka saat ini. Mereka pun mulai memilih-milih pakaian yang ingin mereka gunankan nanti malam. 
“Khe beli baju juga?” tanya Ageng kepada Chandra
“Hm” jawab Chandra singkat.
Entah sudah berapa lama waktu yang di habiskan untuk memilih baju, akhirnya mereka menemukan baju yang mereka inginkan. 
“Aku pake baju ini aja dah warna coklat cocok kayanya. Khe dah dapet baju belum?” 
“Hm, udah. Aku pilih warna biru.” Balas Chandra.
Setelah menemukan baju yang mereka inginkan, mereka pun membayarnya dan kembali pulang untuk mempersiapkan hal-hal yang lain.
Author POV
(Aula SMA)
Matahari mulai terbenam, tak terasa jam sudah menunjukkan pukul 18.50 itu artinya sebentar lagi acara reunian mereka akan di mulai. Riuh ricuh senang dan canda tawa terdengar untuk saling melepaskan rindu.
“Halo… Selamat Malam semuanya, selamat datang di acara reuninya Aksara Pramana angkatan 2014/2015.”  sapa MC  yang memecahkan riuh ricuh mereka semuanya.
“Bagaimana nih kabar kalian semua?”
“BAIKK!!” jawab semua hadirin yang hadir di acara reunian tersebut.
“Nah teman-teman sebelum masuk ke acara inti kita nih, ada persembahan lagu dari salah satu alumni kita yang memiliki suara indah nih. Yaitu Gita Rasa Kirana, kepada yang bersangkutan disilahkan” 
“Ayo kita berikan temukan tangan yang meriah”
Semua hadirin bertepuk tangan untuk Gita. Sementara di sisi lain ada sosok pemuda yang sedang reka ulang kenangan bersama sosok gadis yang sering melantunkan suara indahnya itu.
“Selamat Malam semua, lagu yang akan saya bawakan untuk mengawali acara kita hari ini adalah lagu Remaja dari HIVI!, saya membawakan lagu ini agar kita mengingat kembali masa-masa remaja kita yang saya rasa lebih dominan pada cinta-cintaannya dari pada belajarnya, bener gak nihh temen-temen?” tanya Gita
“BENERRR!!!”
“NGGAKKK!!!”
“Hahaha, kayaknya kita langsung mulai aja ya, selamat mendengarkan” ucap Gita yang menandakan ia akan memulai penampilannya.
Tak lama, setelah instruksi dari Gita kepada panitia, alunan musik pun dimulai dan Gita menyanyikannya dengan sangat merdu membuat Chandra tak dapat berhenti menatap kedepan.
......
Indahnya kisah-kasih kita di masa remaja
Di bawah rayu senja kita di madu bermanja
Tiada masa-masa yang lebih indah dari masa remaja (dari masa remaja)
Seakan dunia
Seakan dunia
Seakan dunia, milik berdua
Masa remaja
Seakan dunia, milik berdua
(Remaja-HIVI!)
Begitu indah Gita melantunkan suara emasnya, semuanya ikut bernyanyi Bersama Gita. 
“Kangenkan sama dia?” Ageng yang peka
Pemuda itu, Chandra hanya diam, tak menjawab pertanyaan dari temannya, dan hanya memandangi gadis cantik yang sedang bernyanyi itu.
“Malah melamun” karena terlalu fokus, pertanyaan Ageng pun diacuhkan
“Bacott” kesel Chandra
“Dihh, marah-marah gak jelas” balas Ageng
“Wahhh penampilan yang sangat keren yaa, benar-benar membawa Kembali rasa-rasa saat SMA dulu, hmm kira-kira masih ada yang gamon gak ya sama mantan-mantannya duluuu”
“ADAAAA, SI CHANDRA” teriak Ageng, hanya dia saja yang berteriak sehingga semua orang menatap padanya dan Chandra.
“Waduhhh, buat mantannya Chandra dengerin nihh, ada yang gamonin” ucap pembawa acara yang membuat semua hadirin terkekeh.
Acara pun kembali berjalan sesuai dengan susunan acara yang sudah dibuat, ada yang menari, quiz, pemotongan tumpeng, dll.
“Hai, lama gak ketemu ya” ucap Gita menyapa gemanya.
“Eh aku dipanggil kesana, duluan ya bro, sukses” alibi Ageng untuk menjauh dari dua orang itu.
“Hai, yah kayaknya pas kelulusan deh terakhir kita ketemu, kabar kamu gimana?” balas Chandra untuk sapaan Gita.
“Iyaa... Dahh lamaa banget ternyata, kabarku ya gini-gini aja sih, baik, sehat, ceria, kayak biasaa hehe, kamu gimana?”
“Hahaha... Aku juga kayak biasanya, Chandra yang kuat dan sehat” Chandra dengan nada sedikit angkuh.
“Bisa aja kamu, kalo tante gimana kabarnya? Masih sehat kan, aku kangen dehh.” Gita mengingat ibunya Chandra karena semasa mereka pacaran dahulu, Gita dekat dengan ibunya Chandra, Tante Kadek.
“Baik kokk, ayo kerumah biar ketemu sama mamah” ajak Chandra, mencari kesempatan dalam kesempitan
“Syukurlahh, boleh tuhh, tante kapan kosongnya biar aku samperin?” Gita sangat senang mendengar kabar Tante Kadek yang baik-baik saja, terlebih lagi kalau anaknya secara tidak langsung mengizinkannya untuk bertemu dengan ibunya.
“Besok aja gimana? Aku libur kok, biar sekalian aku jemput dan jalan-jalan” tawar Chandra.
“Hmm boleh dehh, jam berapa ya enaknya?” tanpa pikir panjang Gita mengiyakan, lumayan lepas rindu dalam hati Gita.
“Pagi aja gimana? Mama siang sampai malam mau pergi acara” saran Chandra agar lebih lama bersama Gita.
“Okayy, nanti aku chat kamu ya alamat rumahnya Widya, aku nginap dirumah dia soalnya” Gita mengiyakan semua tawaran Chandra.
“Okayy, eh kayaknya kamu di panggil deh, masih mau tampilin lagu selanjutnya kan?”
“Oh iyaa, syukur kamu ingetinn, aku masuk dulu yaa sampai ketemu  besokk” Gita masuk sambil melambaikan tangan dan tersenyum pada Chandra yang hanya tertegun dengan cantiknya wanita yang perlahan meninggalkannya masuk kedalam ruangan itu.
Chandra POV
“Cantik bangett ya bro, tau gitu aku pacarin juga tuh” Ageng yang entah datang dari mana membuat lamunan Chandra buyar.
“Aku pukul khe ya” ucapku dengan kepalan tangan yang siap kulayangkan pada Ageng
“Canda-candaa, yaelah gitu aja marah” Ageng sedikit ngeri saat kubilang begitu karena badanku lebih besar darinya.
Tak lama kemudian, alunan musik mulai terdengar menandakan Gita akan kembali memerdukan sekitar dengan suara emasnya.
....
You still look like a movie, 
you still sound like a song
My God this reminds me 
of when we were young
Let me photograph you in this light 
in case it is the last time
That we might 
be exactly like we were 
before we realised
We were sad of getting old, 
it made us restless
It was just like a movie, 
it was just like a song
when we were young
(When We Were Young-Adele)
Lagu yang dibawakan Gita di penghujung acara ini adalah lagu dari Adele berjudul When We Were Young, yang membuat banyak hadirin tersentuh dengan maknanya, bahkan Chandra pun menyadarinya, ia tau makna lagu ini.
Gita POV
(Rumah Widya)
“Iyaa, nanti masuk di gang ketiga terus lurus aja sampai ketemu rumah warna ungu” Instruksiku pada Chandra yang sedang dalam perjalanan menjemputku.
Sebenarnya aku sudah mengirimkan alamat rumah Widya padanya tetapi entah kenapa ia diarahkan menuju gang buntu, ternyata tidak selamanya harus berharap pada teknologi. Setelah 1 jam Chandra keliling mencari jalan lain, ia akhirnya menelponku.
“Ohh okay, ini sudah kelihatan rumahnya” sungguh sangat gesit Chandra mengemudikan mobilnya.
“Niceee, ini aku keluarr yaa” ucapku dan langsung mematikan panggilan itu.
“Widd, aku pergi dulu yaaa” pamitku pada Widya.
“Iyaa, jangan pulang kalau belum balikan yaa” balas Widya sambil tercengir mengejekku.
“Gabakalann pulang dong kalo gituu, dah ahh aku pergi, byee” gurauku menimpal ejekannya dan langsung keluar dari rumah itu.
Sesampainya aku diluar, Chandra sudah menunggu diluar mobil sembari tersenyum kepadaku. Sungguh sangat indah pemandangan didepanku ini, andai saat ini 
aku berani pasti aku akan langsung mengeluarkan handphone ku dan memotret sosok lelaki didepanku ini.
“Haii, sorry lama tadi pamitan dulu sama Widya, biasa agak rempong dikit hehe” kuberikan alasan yang sesungguhnya karena aku tidak mau berbohong untuk hal-hal kecil.
“Nggak kok, baru juga aku keluar dari mobil, kita mau langsung ketemu mamah atau sarapan dulu?” tanya Chandra. 
Aku sedikit membodohi diriku karena lupa membeli buah tangan tadi malam. Terlebih lagi aku lupa sarapan, kan bisa malu kalau perut ini merengek meminta asupan saat aku sedang bersama Tante Kadek dan Chandra
“Hmm makan dulu deh, aku agak lapar sihh, sama habis itu beli buah tangan buat Tante Kadek dulu, gapapa kan?” pintaku tanpa tau malu.
“Iyaa gapapa, ayo masuk biar kita jalan sekarang” Chandra sambil terkekeh mengiyakan dan membukakan pintu mobil untukku.
“Thank you Ndraa” terima kasihku atas perlakuan Chandra yang manis.
Author POV
(Mobil Chandra)
Perjalanan pun terasa heningnya, canggung yang dirasakan kedua insan itu melingkupi atmosfer disekitar mereka.
“Kamu sekarang kerja dimana?” tanya Gita untuk menghilangkan canggung disekitar mereka.
“Aku kerja di perusahaan arsitek, kalo kamu?” tanya Chandra
“Ohh, bener juga sih kan kamu pernah bilang dulu kamu mau jadi arsitek, gambaran kamu juga bagus-bagus. Sekarang aku kerja di kantor jasa akuntansi” jawab Gita
“Wahh keren tuhh, kayaknya asik deh kerjaan kamu” Chandra memberikan pernyataan yang langsung dijawab oleh Gita.
“Yahh, dibilang asik sih gak juga yaa, kebanyakan stressnya aku tuhh, kerjaan kamu lebih asik deh kayaknya” begitulah jawaban Gita.
“Sama kayak kamu lah jawabanku, banyakan stressnya, klien biasanya banyak maunya” jawab Chandra sambil terkekeh.
Akhirnya selama diperjalanan itu mereka membahas tentang kerjaan mereka, hingga tibalah mereka disatu rumah makan yang sering mereka kunjungi saat pacaran dahulu.
“Ehh ini rumah makan Ibu Mini gak sihh??” tanya Gita yang langsung mengtahui lokasinya saat itu.
“Iyaa, aku gak tau kamu mau makan apa, kalau ditanya juga jawabnya pasti terserah jadi aku bawa kesini aja, biar kamu bisa makan Ayam Betutu kesukaan kamu” jawab Chandra yang membuat Gita sedikit luluh.
“Ihh kamu tau aja aku lagi pengen ayam ini, di Surabaya aku coba makan Ayam Betutu juga tapi gak ada yang sama kayak disini” Gita tampaknya sangat berterimakasih karena sudah dibawa ke tempat kesukaannya sampai-sampai ia sedikit curhat akan apa yang ia rasakan di Surabaya.
“Hahaha… Iya dongg, Chandra gituloh” Chandra menjawab itu dengan rasa bangga dihatinya seperti memenangkan hadiah utama di sebuah event.
“Dah yok masuk, dah laper kann” ajak Chandra agar mereka cepat masuk dan menyantap makanan yang mereka inginkan.
“Hehee, ayokkk” Gita pun secara tidak sadar langsung menarik tangan Chandra yang membuat pemuda itu sontak terkaget dan diakhiri dengan tersenyum.
Author POV
(Toko Kue)
“Gitt ayoo, sebelum kesiangann” ajak Chandra.
Masalahnya, sudah hampir sejam mereka berada di toko kue itu. 
“Ihh sabar Ndraa, aku bingungg kasih Tante apa, ini atau yang ini Ndraa?” tanya Gita sambil menunjuk kue lapis dan kue nastar.
“Yang ini aja deh Gitt, mamah kayaknya suka yang ini” jawab Chandra menunjuk Kue Lapis.
“Yaudah, mbak saya ambil kue lapisnya 2 box ya” ucap Gita kepada salah satu staff di toko itu.
“Aku bayar dulu ya, kamu kalo mau tunggu diluar gapapaa” ucap Gita selagi berjalan menuju meja kasir.
Chandra pun keluar menuju mobil dan langsung menyalakan mesin serta ac agar saat Gita masuk ia tidak kepanasan.
Sesampainya dimobil Gita langsung mengarahkan ac mobil ke arah dirinya.
“Duhh panas bangett yaa, aku arahin acnya ke aku gapapa ya?” ijin Gita walaupun ia sudah melakukannya.
“Iyaa, terserah kamu deh mau ngapainn, gapapa kokk” balas Chandra sedikit menggoda Gita.
Gita terkekeh mendengar tuturan kata Chandra, ia jadi ingat kalau Chandra dulu suka mengucapkan sesuatu yang manis padanya.
Setelah itu, Chandra pun mulai mengemudikan mobilnya menuju kediaman mamahnya. Selama diperjalanan pun mereka tidak canggung lagi melainkan saling bercanda gurau.
“Eh tapi iya tauu, masa waktu aku pergi ke kebun binatang ada panda tapi ternyata itu anjing yang di cat” kata Gita berdasarkan pengalamannya berkunjung ke kebun binatang waktu itu.
“Dihh aneh-aneh ajaa, padahal kalo gak mampu dapat panda gak usah dipaksain gak sih?” Chandra membalasnya dengan sedikit kesal karena ia tidak suka sesuatu yang bersifat dipaksakan.
“Betul-betul, tapi yaudahlah biarkan aja”
“Ehh tapi kamu tau gak, masa ya si itu…”
Yap, perjalanan mereka pun berlanjut dengan pergibahan yang dipimpin oleh wanita Surabaya itu.
(Rumah Tante Kadek)
“Wahh, rumah kamu gak berubah yaa, masih fresh bangett” kagum Gita sembari tersenyum dan menarik nafas dalam-dalam.
Mamahku memang seorang yang suka sekali dengan tanaman, tak heran jika Gita berkata bahwa rumahku terlihat fresh hanya dengan menghirup udara disekitarnya saja.
Aku tersenyum memandanginya “Iya dong, mamah masih suka ngerawat pekarangannya, bisa-bisa pekarangan ini yang jadi anaknya, bukan aku hahaha…”
“Aku setuju sihh, kalo aku jadi Tante Kadek juga bakalan pilih ngerawat pekarangan dari pada anak tengil kayak kamu” gurau Gita yang membuatku memasang wajah sok sedih.
Dengan nada sedikit merengek aku berkata “Kok gitu sihh, padahal aku anak yang baik hati dan tidak sombong”  membuat Gita langsung memasang ekspresi geli dan mengernyit.
“Ihhh, gelii bangetttt, Chandra diam gakkk” Gita berkata sambil mengelus kedua lengannya menandakan ia geli.
“Hahahaa….” Gelak tawaku ketika melihat dia.
“Chandraa??” suara perempuan terdengar dari arah pintu rumah.
“Mahh” sapaku langsung menghampiri Mamah, tidak lupa kuajak Gita juga.
“Halo tantee...” Gita juga menyapa Mamah dengan tersenyum manis
Mamah tampak kebingungan tetapi langsung ingat siapa sosok perempuan yang kubawa ini.
“Astagaa, Gita yaaa? Ya ampun sayangg, lama banget gak ketemu” ucap mama langsung memeluk Gita.
Sambil membalas pelukan mamah, Gita menanyakan kabar mamah “Iya tantee, kangen bangett sama Tantee, apa kabar tann?”
“Baikk sayangg, duhh kamu makin cantik aja” puji mamahku dan kulihat Gita lansung merona malu.
“Yukk masuk sayangg, tante ada buat jajan didalamm” sambil mamah tarik Gita masuk.
Aku hanya bisa menghela nafas, sungguh bagaimana bisa anaknya ditinggalkan diluar dan anak orang lain diajak masuk, tapi selain itu aku tetap senang melihat pemandangan didepanku. Tanpa lama-lama aku juga masuk kedalam dan melihat rumah yang tetap bersih, entah bagaimana caranya, sepertinya mamahku membersihkan rumah ini setiap hari. Mungkin aku harus lebih sering menginap dirumah ini dari pada di apartemenku.
Sesampai didalam aku melihat mamah dan Gita sudah saling bercengkerama bagai sudah sedekat ibu dan anak kandung.
Sambil duduk di sofa aku menyela obrolan mereka “Akrab bener diliat-liat” 
“Iya dongg, kan udah lama gak ketemuu” sahut mamah
“Oh Iya tan, ini aku ada beli kue buat tante, semoga suka yaa” Gita menyerahkan tas berisi Kue Lapis yang dibelinya tadi.
“Duhh kamu repot-repot, tante kan jadi enak, eh ngga enak maksudnya” mamahku sedikit bergurau dan membuat Gita juga terkekeh, padahal tidak ada yang lucu.
Obrolan kembali berlanjut, entah apa saja yang dibahas aku hanya mendengarkan dengan sesekali kutimpal juga. Tak terasa jam sudah menunjukkan pukul 12.30 yang menandakan bahwa mamah harus bersiap-siap karena akan pergi ke suatu acara. 
“Ehh tante siap-siap dulu ya sayang, mau pergi acara dulu. Maaf tante gak bisa suguhin apa-apa” ucap mamahku padahal dimeja depan kami sudah ada berbagai macam jajan yang mamah buat dan minuman. 
Gita langsung mengiyakan “Iya tantee, makasihh banyak ya tantee maaf ngerepotin”
“Gak ngerepotin kok sayang, sering-sering datang kesini sama Chandra yaa, kalau bisa statusnya jangan hanya temen aja pas datang lagi, hahaha….” Ucap mamahku.
KANNNN.... Aku paling gak suka kalo mamahku sudah begini, kenapa sih orang tua di Indonesia ceplas-ceplos banget. Sekarang Gita hanya bisa tersenyum, dan  sempat saling tatap denganku. Aku hanya bisa terkekeh dan menimpali omongan mamahku dengan sedikit kesal.
“Mamah apasihh” kesalku
“Kan Cuma bercandaa, yakan Gita?” pertanyaan yang hanya dijawab senyuman kecil dari Gita.
“Kalau gitu kita sekalian pamit aja mahh, mau lanjut ketempat lain” aku ingin segera pergi dari sini, takutnya makin kesana-kesini nanti.
“Yaudah, kalian hati-hati yaa” sepertinya mamahku juga buru-buru karena ia langsung mengiyakan omonganku.
“Pamit dulu ya tante” Gita menyalim tangan mamahku
“Pergi dulu mahh” aku mencium pipi mamahku.
Setelah itu mamah mengantarkan kami ke depan pintu rumah dan kami langsung menaiki mobil. Aku sempatkan diri untuk mengklakson dahulu sebelum keluar dari halaman rumah itu.
Gita POV
(Didalam Mobil)
Duhh siall, canggung banget sihh, bahas apa ya biar gak canggung, oh aku tau…
“Kita mau kemana Ndraa? tadi kamu bilang mau pergi ke suatu tempat” tanyaku karena memang kami tidak ada plan mau pergi kemana-mana selain kerumah Tante Kadek.
“Gak tau sih, aku tadi bilang gitu supaya cepat keluar daripada mamah makin ngelantur omongannya” Chandra menjawab dengan sangat realistis yang membuatku menganggukkan kepala tanda setuju.
“Tapi bosan juga sih, mau ke mall gak? Nonton, makan, atau main kalo kamu mau” lanjut Chandra
Aku sedikit menimbang-nimbang namun saya kalau sudah liburan dan diam dirumah saja “Boleh deh, bosan juga dirumah”
“Okay kita ke Mall yaa” Chandra tersenyum melihatku dan aku juga tersenyum melihatnya. Debaran jantungku sedikit lebih kencang ketika itu terjadi, aku jadi takut akan terulang kembali.
Author POV
(Mall)
Sesampainya di mall, Gita dan Chandra menjelajah semua toko yang terdapat didalamnya, mereka makan, nonton film, dan bermain permainan. Semuanya dilakukan seperti mengenang masa pacaran mereka dahulu. Sampai-sampai mereka tidak ingat waktu yang dimana sekarang menunjukkan pukul 17.30.
“Sudah jam segini, mau ke pantai gak? Sunsetan enak nih kayaknya” ajak Chandra 
“Wihh asikk tuh, ayokkk” girang Gita.
Selanjutnya mereka berdua keluar dari mall dan bergegas menuju pantai terdekat. 
Sesampainya di pantai, dua insan itu langsung mencari spot terbaik untuk menikmati matahari terbenam. Setelah mendapat tempat dan duduk, barulah percakapan mereka dimulai.
Chandra menutup matanya “Ahh, segar banget ya anginnya”
Gita yang melihat Chandra menutup matanya juga ikutan untuk menikmati seperti cara Chandra “iyaa seger bangett, sudah lama aku nggak menikmati duniawi kayak gini”
Chandra membuka matanya dan menatap gadis disampingnya ini, sangat cantik batinnya. Ia mengingat masa-masa akhir SMA, saat ia diputuskan oleh Gita hanya karena mereka harus fokus dalam belajar dan juga mereka berbeda, keyakinan berbeda dan mereka tau bahwa itu sulit untuk disatukan. Alasan yang terdengar biasa saja namun siapapun yang mengalaminya pasti merasakan sakit yang dalam. Banyak orang yang mengalaminya dan banyak juga yang merelakan kepercayaannya demi sebuah cinta, Chandra dan Gita bukan orang yang seperti itu, mereka mengingat siapa yang selalu menjadi tempatnya menangis, meminta, dan memohon, mereka bukanlah kacang yang lupa kulitnya, mereka akan mendahulukan kepercayaannya dari pada duniawi.
“Gitt, andai aja kita seagama ya, pasti sehari-hari kita bisa nikmatin senja, main bareng, makan bareng” ucap Chandra dengan menatap Gita.
Gita membuka matanya dan menatap Chandra “Udahlah Ndraa gak usah dibahas lagi, nanti aku gamon nih, hahhaa…” Sempat-sempatnya Gita bercanda disaat seperti ini.
“Hahaha, bisa aja kamu, tapi sekarang kita fine-fine aja kan ya” Chandra memastikan.
“Iya dongg bestiee, hahaha” rasanya Gita sudah berdamai dengan perasaannya kepada Chandra dan seharusnya Chandra pun begitu.
Gita POV
(Bandara)
“Hikss Gitaaa, kenapa cepett bangett baliknyaa” aku mengusap-usap punggung Widya untuk menenangkan sahabatku ini.
Aku menanggapinya dengan sedikit berlinang air mata “Cutikuu udah habiss Widd, ntar kalo liburan aku balik lagi yaa” 
Sejujurnya aku juga sedikit tidak rela untuk harus kembali ke realita kehidupanku, aku masih ingin berlibur di Bali, bersenang-senang dan memanjakan diriku dengan berbagai kesenangan yang tentunya tidak bersifat haram.
“Beneran balik lagi kan?” suara berat ini terdengar sangat familiar.
Aku langsung melepaskan pelukan Widya dan membalik badanku, benar saja, saat ini Chandra sudah ada didepanku dengan memegang sebuah kotak kecil dan paper bag yang aku yakini isinya adalah oleh-oleh khas Bali.
“Kok kamu bisa ada disinii Ndraa?” tanyaku heran.
“Kan kamu yang kasih tau kalo mau balik hari ini, jadi aku samperin aja sekalian antar titipan mamah” jawab Chandra sambil menyerahkan tote bag oleh-oleh itu.
“Astagaa, tante sampai repot-repot ginii, sampaikan  salamku yaa, terima kasih banyakk tantee” kuminta Chandra menyampaikan salamku sebab kutahu Tante Kadek tidak ikut karena berbagai acara tau kesibukan lainnya, biasa orang tua ada saja acaranya.
“Iyaa nanti aku sampaikan, sama ini oleh-oleh dari aku” kata Chandra sambil memberikan kotak kecil yang kulihat tadi.
“Apa nih Ndraa?” aku heran karena kotak itu sangat kecil.
“Buka aja nanti dipesawat” jawab Chandra
“Good Morning, ladies and gentleman. This is a pre-boarding announcement for passengers of Selamat Airlines on flight number 127B to Surabaya, we would like to…..” terdengar pengumuman keberangkatan pesawatku sebentar lagi
“Itu pesawat kamu kan?” tanya Chandra
“Iyaa, aku masuk dulu yaa, makasih banyak Chandraa” seruku pada Chandra yang dibalas dengan anggukan dari pemuda itu.
“Ehemm ehemm” aku tau itu Widya.
“Makasih banyak ya sayanggkuu, aku pulang duluu” ucapku sambil memeluk Widya lagi
“Janji yaaa Gitt” 
“Iyaaa Widd”
Setelah berpelukan dengan Widya, aku langsung mengambil barang-barangku dan melanjutkan dengan check in dan lain-lainnya.
Pesawatku sudah terbang, saat ini aku membuka kotak yang diberikan Chandra. Saat kubuka ternyata isinya adalah sebuah Memory Card, sungguh ini membuatku penasaran apa isi dari benda tersebut. Aku pun mendapatkan ide, Kuambil jarum pentul yang tertancap di hijabku dan kubuka bagian hpku untuk menaruh memory card tersebut di SD Card Slot yang ada pada hpku. Setelah itu aku melihat apa isi dari benda kecil itu. Air mataku mulai berlinang melihat isi benda itu, sebuah folder berjudul ‘GC Memories’ yang berisikan foto, video, dan segala kenangan antara kami berdua tersimpan di dalam folder itu. Jujur tangisanku bukan karena sedih tapi karena aku bangga akan diriku yang berani mengambil keputusan besar sebelum keadaan semakin parah, dan bangga pada kami berdua yang berhasil menahan perasaan satu sama lain hingga bertemu insan yang dapat menggantikan gema yang berbayang dalam hidup kami.
)", "26-12-2019");

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
void tampilIsiCerpen(int indexAkun, Cerpen *c, int halamanAwal = 1);
void menuUtama(int indexAkun);
// ─── History ─────────────────────────────────────────────────

void tambahHistory(int indexAkun, const string &judul, int idCerpen)
{
  akun &user = daftarAkun[indexAkun];

  // Hapus entry lama kalau judul yang sama sudah pernah dibuka
  int idx = -1;
  for (int i = 0; i < user.jumlahRiwayat; i++)
  {
    if (user.riwayat[i].judul == judul)
    {
      idx = i;
      break;
    }
  }
  
  if (idx != -1)
  {
    // Geser elemen ke kiri untuk hapus
    for (int i = idx; i < user.jumlahRiwayat - 1; i++)
    {
      user.riwayat[i] = user.riwayat[i + 1];
    }
    user.jumlahRiwayat--;
  }

  // Tambah di posisi paling depan (geser semua ke kanan)
  if (user.jumlahRiwayat < MAX_HISTORY)
  {
    for (int i = user.jumlahRiwayat; i > 0; i--)
    {
      user.riwayat[i] = user.riwayat[i - 1];
    }
    
    user.riwayat[0].judul = judul;
    user.riwayat[0].waktu = getCurrentTimestamp();
    user.riwayat[0].idHistory = user.jumlahRiwayat + 1;
    user.riwayat[0].idCerpen = idCerpen;
    user.riwayat[0].halamanTerakhir = 1;
    user.jumlahRiwayat++;
  }
}

void updateHalamanHistory(int indexAkun, const string &judul, int halaman)
{
  akun &user = daftarAkun[indexAkun];
  for (int i = 0; i < user.jumlahRiwayat; i++)
  {
    if (user.riwayat[i].judul == judul)
    {
      user.riwayat[i].halamanTerakhir = halaman;
      break;
    }
  }
}

void menuHistory(int indexAkun)
{
  while (true)
  {
    clearScreen();
    tampilHeader("HISTORY");

    akun &user = daftarAkun[indexAkun];
    if (user.jumlahRiwayat == 0)
    {
      cout << COLOR_PINK << R"(
 /\_/\
( o.o )
 > ^ <
Belum ada riwayat cerpen yang dibuka.)" << COLOR_RESET;
      cout << "\n  Tekan Enter untuk kembali...";
      showKursor();
      cin.ignore();
      cin.get();
      return;
    }

    //array pointer untuk navigasi
    vector<history*> items;
    for (int i = 0; i < user.jumlahRiwayat; i++)
    {
      items.push_back(&user.riwayat[i]);
    }

    int selected = 0;
    int total = items.size();

    hideKursor();
    while (true) {
      clearScreen();
      tampilHeader("HISTORY");

      // Tampilkan daftar pake highlight
      for (int i = 0; i < total; i++) {
        if (i == selected) {
          cout << COLOR_GREEN << "  > " << COLOR_RESET;
          cout << COLOR_WHITE << items[i]->judul << COLOR_RESET;
        } else {
          cout << "    " << items[i]->judul;
        }
        cout << COLOR_CYAN << "  (" << items[i]->waktu << ")\n" << COLOR_RESET;
      }

      cout << COLOR_CYAN << "\n  Gunakan panah atas/bawah, Enter untuk pilih, 0 untuk kembali\n" << COLOR_RESET;

      int ch = _getch();
      if (ch == 0 || ch == 224) { // tombol panah
        ch = _getch();
        if (ch == 72) { // atas
          selected = (selected - 1 + total) % total;
        } else if (ch == 80) { // bawah
          selected = (selected + 1) % total;
        }
      } else if (ch == 48) {
        showKursor();
        return;   // kembali ke menu utama
      }else if (ch == 13) { // Enter(kk)
        // Pilih item (selec kk)
        history *h = items[selected];
        Cerpen *c = cariCerpenById(h->idCerpen);
        if (c == nullptr) {
          cout << COLOR_RED << "\n  Cerpen tidak ditemukan!\n" << COLOR_RESET;
          cout << "  Tekan Enter...";
          showKursor();
          cin.ignore();
          cin.get();
          break; // kembali ke menu history? >_<
        }
        // Tampilkan 3 pilihan
    vector<string> menu = {
      "Buka profile cerita",
      "Baca page yang dibaca terakhir kali (halaman " + to_string(h->halamanTerakhir) + ")",
      "Cancel"
    };
    int selected = 0;
    bool selesai = false;
    hideKursor();
    while (!selesai)
    {
        clearScreen();
        tampilHeader("PILIHAN");
        cout << COLOR_WHITE << "\n  Cerpen: " << c->judul << "\n\n";

        // mewarnai MENU xixi
        for (int i = 0; i < menu.size(); i++)
        {
            string text = menu[i];

            // 🔵 kalau dipilih ada highlightnya
            if (i == selected)
            {
                cout << COLOR_BLUE << " > " << text << COLOR_RESET << "\n";
            }
            else
            {
                // 🔴 ini biar log out merah
                if (i == 3)
                    cout << COLOR_RED << "   " << text << COLOR_RESET << "\n";
                else
                    cout << COLOR_YELLOW << "   " << text << COLOR_RESET << "\n";
            }
        }
        int result = menuSelector(menu, selected); //biar bisa selec kk(arrow)
        if (result < 0)
        {
            int choice = -result - 1;
            switch (choice)
            {
            case 0: showKursor(); cerpenInfo(indexAkun, c); hideKursor(); selesai = true; break;
            case 1: showKursor(); tampilIsiCerpen(indexAkun, c, h->halamanTerakhir); hideKursor(); selesai = true; break;
            case 2: selesai = true; break;
            }
        }
        else
        {
            selected = result;
        }
    }
    }
  }
}
}
// ─── Bookmark / Favorit ──────────────────────────────────────

Bookmark *cariBookmark(int indexAkun, const string &judul)
{
  akun &user = daftarAkun[indexAkun];
  for (int i = 0; i < user.jumlahBookmark; i++)
  {
    if (user.bookmark[i].judul == judul)
      return &user.bookmark[i];
  }
  return nullptr;
}

void tambahBookmark(int indexAkun, const string &judul)
{
  akun &user = daftarAkun[indexAkun];
  if (user.jumlahBookmark >= MAX_BOOKMARK) return;
  
  user.bookmark[user.jumlahBookmark].judul = judul;
  user.bookmark[user.jumlahBookmark].idBookmark = user.jumlahBookmark + 1;
  user.jumlahBookmark++;
}

void hapusBookmark(int indexAkun, const string &judul)
{
  akun &user = daftarAkun[indexAkun];
  int idx = -1;
  
  for (int i = 0; i < user.jumlahBookmark; i++)
  {
    if (user.bookmark[i].judul == judul)
    {
      idx = i;
      break;
    }
  }
  
  if (idx != -1)
  {
    // Geser elemen ke kiri
    for (int i = idx; i < user.jumlahBookmark - 1; i++)
    {
      user.bookmark[i] = user.bookmark[i + 1];
    }
    user.jumlahBookmark--;
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
        if (user.jumlahBookmark == 0)
    {
      cout << COLOR_CYAN << "\n  Belum ada cerpen favorit.\n"
           << COLOR_RESET;
      cout << "\n  Tekan Enter untuk kembali...";
      showKursor();
      cin.ignore();
      cin.get();
      return;
    }

           // kumpulinn semua bookmark ke vector biar gampang diakses
        vector<Bookmark*> items;
        for (int i = 0; i < user.jumlahBookmark; i++)
        {
            items.push_back(&user.bookmark[i]);
        }

        int selected = 0;
        int total = items.size();

        hideKursor();
        while (true)
        {
            clearScreen();
            tampilHeader("FAVORIT");

            // Tampilkan daftar dengan highlight
            for (int i = 0; i < total; i++)
            {
                if (i == selected)
                {
                    cout << COLOR_GREEN << "  > " << COLOR_RESET;
                    cout << COLOR_WHITE << items[i]->judul << COLOR_RESET;
                }
                else
                {
                    cout << "    " << items[i]->judul;
                }

                // Tambahkan info penulis and genre
                Cerpen *c = cariCerpenByJudul(items[i]->judul);
                if (c != nullptr)
                    cout << COLOR_CYAN << " - " << c->penulis << " (" << c->genre << ")" << COLOR_RESET;

                cout << "\n";
            }

            cout << COLOR_CYAN << "\n  Gunakan panah atas/bawah, Enter untuk pilih, 0 untuk kembali\n" << COLOR_RESET;

            int ch = _getch();

            if (ch == 0 || ch == 224) // Tombol panah
            {
                ch = _getch();
                if (ch == 72) // Atas
                    selected = (selected - 1 + total) % total;
                else if (ch == 80) // Bawah
                    selected = (selected + 1) % total;
            }
            else if (ch == 13) // Enter buat pilih bookmark
            {
                Bookmark *bm = items[selected];
                Cerpen *c = cariCerpenByJudul(bm->judul);
                if (c != nullptr)
                {
                    showKursor();
                    cerpenInfo(indexAkun, c); // buka profile cerpen
                }
                else
                {
                    cout << COLOR_RED << "\n  Cerpen tidak ditemukan!\n" << COLOR_RESET;
                    cout << "  Tekan Enter...";
                    showKursor();
                    cin.ignore();
                    cin.get();
                }
                break; // keluar dari inner loop, kembali ke daftar favorit
            }
            else if (ch == '0') // Kembali ke menu utama
            {
                showKursor();
                return;
            }
        }
    }
}
// ─── Baca Cerpen ─────────────────────────────────────────────

void tampilSemuaCerpen()
{
  if (jumlahCerpen == 0)
  {
    cout << COLOR_CYAN << "\n  Belum ada cerpen.\n"
         << COLOR_RESET;
    return;
  }

  for (int i = 0; i < jumlahCerpen; i++)
  {
    cout << COLOR_YELLOW << "  [" << daftarCerpen[i].id << "] " << COLOR_RESET
         << daftarCerpen[i].judul << "\n";
  }
}
void tambahKomentar(Cerpen *c, int rating, const string &nama, const string &isi)
{
  if (c->jumlahKomentar >= MAX_KOMENTAR) return;
  
  // Geser semua komentar ke kanan untuk insert di depan
  for (int i = c->jumlahKomentar; i > 0; i--)
  {
    c->daftarKomentar[i] = c->daftarKomentar[i - 1];
  }
  
  c->daftarKomentar[0].rating = rating;
  c->daftarKomentar[0].nama = nama;
  c->daftarKomentar[0].isi = isi;
  c->daftarKomentar[0].waktu = getCurrentTimestamp();
  c->jumlahKomentar++;
}

void menuRatingKomentar(int indexAkun, Cerpen *c)
{
  clearScreen();
  tampilHeader("RATING & KOMENTAR");
  cout << COLOR_WHITE << "\n  Cerpen : " << COLOR_RESET << c->judul << "\n";

    // Pilih rating dengan arrow
  vector<string> ratingMenu = {
    "1 - Kurang",
    "2 - Cukup",
    "3 - Baik",
    "4 - Sangat Baik",
    "5 - Luar Biasa",
    "Batal"
  };

  int selected = 0;
  hideKursor();

  while (true)
  {
    clearScreen();
    tampilHeader("RATING & KOMENTAR");
    cout << COLOR_WHITE << "\n  Cerpen : " << COLOR_RESET << c->judul << "\n";
    cout << "\n  Pilih Rating:\n\n";

    for (int i = 0; i < ratingMenu.size(); i++)
    {
      if (i == selected)
      {
        if (i == 5) // Batal
          cout << COLOR_RED << "  > " << ratingMenu[i] << COLOR_RESET << "\n";
        else
          cout << COLOR_YELLOW << "  > " << ratingMenu[i] << COLOR_RESET << "\n";
      }
      else
      {
        if (i == 5)
          cout << COLOR_RED << "    " << ratingMenu[i] << COLOR_RESET << "\n";
        else
          cout << COLOR_CYAN << "    " << ratingMenu[i] << COLOR_RESET << "\n";
      }
    }

    int result = menuSelector(ratingMenu, selected);

    if (result < 0)
    {
      int choice = -result - 1;
      if (choice == 5) // Batal
      {
        showKursor();
        return;
      }
            else // Pilih rating
      {
        int rating = choice + 1;
        
        // Input komentar
        clearScreen();
        tampilHeader("RATING & KOMENTAR");
        cout << COLOR_WHITE << "\n  Cerpen : " << COLOR_RESET << c->judul << "\n";
        cout << COLOR_YELLOW << "  Rating : " << ratingMenu[choice] << COLOR_RESET << "\n\n";

        string isiKomentar;
        cout << "  Tulis komentar : ";
        showKursor();
        
        // Flush input buffer sebelum getline
        cin.sync();
        getline(cin, isiKomentar);

        if (isiKomentar.empty())
        {
          cout << COLOR_RED << "\n  Komentar tidak boleh kosong!\n" << COLOR_RESET;
          cout << "  Tekan Enter...";
          cin.get();
          return;
        }

        tambahKomentar(c, rating, daftarAkun[indexAkun].nama, isiKomentar);

        cout << COLOR_GREEN << "\n  Rating dan komentar berhasil disimpan!\n" << COLOR_RESET;
        cout << "  Tekan Enter...";
        cin.get();
        return;
      }
    }
    else
    {
      selected = result;
    }
  }
}

void tampilKomentar(int indexAkun, Cerpen *c)
{
  while (true)
  {
    clearScreen();
    tampilHeader("KOMENTAR");
    cout << COLOR_WHITE << "\n  Cerpen : " << COLOR_RESET << c->judul << "\n";

    if (c->jumlahKomentar == 0)
    {
      cout << COLOR_CYAN << "\n  Belum ada komentar untuk cerpen ini.\n"
           << COLOR_RESET;
    }
    else
    {
      cout << "\n";
      for (int i = 0; i < c->jumlahKomentar; i++)
      {
        cout << COLOR_YELLOW << "  " << c->daftarKomentar[i].nama << COLOR_RESET
             << " [" << c->daftarKomentar[i].rating << "/5]"
             << COLOR_CYAN << "  " << c->daftarKomentar[i].waktu << "\n"
             << COLOR_RESET
             << "  " << c->daftarKomentar[i].isi << "\n\n";
      }
    }

    // Menu dengan arrow select
    vector<string> menu = {
      "Beri Rating & Komentar",
      "Kembali"
    };

    int selected = 0;
    hideKursor();

    while (true)
    {
      // Simpan posisi kursor setelah list komentar
      cout << "\033[s";

      // Tampilkan menu
      for (int i = 0; i < menu.size(); i++)
      {
        if (i == selected)
        {
          if (i == 1)
            cout << COLOR_RED << "  > " << menu[i] << COLOR_RESET << "\n";
          else
            cout << COLOR_YELLOW << "  > " << menu[i] << COLOR_RESET << "\n";
        }
        else
        {
          if (i == 1)
            cout << COLOR_RED << "    " << menu[i] << COLOR_RESET << "\n";
          else
            cout << COLOR_YELLOW << "    " << menu[i] << COLOR_RESET << "\n";
        }
      }

      cout << COLOR_CYAN << "\n  Gunakan panah atas/bawah, Enter untuk pilih\n" << COLOR_RESET;

      int result = menuSelector(menu, selected);

      if (result < 0)
      {
        int choice = -result - 1;
        if (choice == 0) // Beri Rating & Komentar
        {
          showKursor();
          menuRatingKomentar(indexAkun, c);
          break; // Refresh list komentar
        }
        else if (choice == 1) // Kembali
        {
          showKursor();
          return;
        }
      }
      else
      {
        selected = result;
        // Restore posisi dan hapus menu lama
        cout << "\033[u";
        cout << "\033[J";
      }
    }
  }
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

    cout << "\n  " << COLOR_CYAN << "Tekan Enter untuk mulai membaca..."
       << COLOR_RESET;
  showKursor();
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

void tampilIsiCerpen(int indexAkun, Cerpen *c, int halamanAwal)
{
  tampilLayarBuka(c);

  const int BARIS_PER_HALAMAN = 20;
  vector<string> baris = pecahBaris(c->isi, 68);
  int totalHalaman = ((int)baris.size() + BARIS_PER_HALAMAN - 1) / BARIS_PER_HALAMAN;
  if (totalHalaman == 0) totalHalaman = 1;
  int halSaat = halamanAwal;
  if (halSaat < 1) halSaat = 1;
  if (halSaat > totalHalaman) halSaat = totalHalaman;

  vector<string> menuItems;
  vector<int> aksiKode;   // 0: prev, 1: next, 2: komentar, 3: selesai
  int selected = 0;
    
  auto bangunMenu = [&]() {
      menuItems.clear();
      aksiKode.clear();
      if (halSaat > 1) {
            menuItems.push_back("[<-]Halaman Sebelumnya");
            aksiKode.push_back(0);
        }
      if (halSaat < totalHalaman) {
            menuItems.push_back("[->]Halaman Berikutnya");
            aksiKode.push_back(1);
        }
      menuItems.push_back("Komentar");
      aksiKode.push_back(2);
      menuItems.push_back("Selesai Baca");
       aksiKode.push_back(3);
      if (selected >= (int)menuItems.size()) selected = 0;
    };
 
    bangunMenu();
    hideKursor();

    while (true) {
        {
            clearScreen();
            tampilHeader(c->judul);
                        cout << COLOR_CYAN << "  Halaman " << halSaat << " / " << totalHalaman
                 << COLOR_RESET << "\n\n";
 
            int mulai = (halSaat - 1) * BARIS_PER_HALAMAN;
            int akhir = mulai + BARIS_PER_HALAMAN;
            if (akhir > (int)baris.size()) akhir = (int)baris.size();
 
            for (int i = mulai; i < akhir; i++)
                cout << "  " << baris[i] << "\n";
 
            cout << COLOR_CYAN << "\n  ------------------------------------------\n"
                 << COLOR_RESET;
 
            for (int i = 0; i < (int)menuItems.size(); i++) {
                string text = menuItems[i];
                if (i == selected) {
                    cout << COLOR_BLUE << " > " << text << COLOR_RESET << "\n";
                } else {
                    if (i == (int)menuItems.size() - 1)
                        cout << COLOR_RED << "   " << text << COLOR_RESET << "\n";
                    else
                        cout << COLOR_YELLOW << "   " << text << COLOR_RESET << "\n";
                }
            }
 
            int ch = getch();
 
            if (ch == 224) { 
                ch = getch();
                switch (ch) {
                    case 72:   // panah atas
                        selected--;
                        if (selected < 0) selected = (int)menuItems.size() - 1;
                        break;
                    case 80:   // panah bawah
                        selected++;
                        if (selected >= (int)menuItems.size()) selected = 0;
                        break;
                    default: break;
                }
                // Lanjutkan loop untuk refresh menu dengan highlight baru
                continue;
            }
            else if (ch == 13) {   // Enter → pilih item yang sedang di-highlight
                int aksi = aksiKode[selected];
                switch (aksi) {
                    case 0:   // Halaman Sebelumnya
                        if (halSaat > 1) halSaat--;
                        updateHalamanHistory(indexAkun, c->judul, halSaat);
                        break;
                    case 1:   // Halaman Berikutnya
                        if (halSaat < totalHalaman) halSaat++;
                        updateHalamanHistory(indexAkun, c->judul, halSaat);
                        break;
                    case 2:   // Komentar
                        showKursor();
                        tampilKomentar(indexAkun, c);
                        hideKursor();
                        break;
                    case 3:   // Selesai Baca
                        updateHalamanHistory(indexAkun, c->judul, halSaat);
                        showKursor();
                        return;   // keluar dari fungsi
                }
                // Bangun ulang menu karena item nav bisa berubah (misal
                // "Halaman Sebelumnya" muncul/hilang) tergantung halaman baru
                bangunMenu();
            }
            else {
                continue;
            }
        }
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
      cout << COLOR_CYAN << "Belum ada rating" << COLOR_RESET << "\n";
    else
      cout << rating << " / 5\n";

    cout << COLOR_WHITE << "\n  Preview:\n"
         << COLOR_RESET << "  " << buatPreview(c->isi) << "\n";

    //ini batas perubahannya
    cout << "\033[s";   // simpan posisi kursor (setelah sinopsis)

    bool favorit = cariBookmark(indexAkun, c->judul) != nullptr;
    vector<string> menuItems;
    vector<int> aksiKode;
    menuItems.push_back("Baca Cerpen");
    aksiKode.push_back(1);

    if (favorit) {
        menuItems.push_back("Hapus dari Favorit");
    } else {
        menuItems.push_back("Tambah ke Favorit");
    }
    aksiKode.push_back(2);

    menuItems.push_back("Kembali");
    aksiKode.push_back(0);

    int selected = 0;

    hideKursor();
    while (true) {
        // modif di sini biar atas ga ilang pas pakai kusor
        // Ganti clearScreen() dan cetak ulang judul dengan restore posisi + hapus area menu
        cout << "\033[u";   // kembali ke posisi setelah sinopsis
        cout << "\033[J";   // hapus dari posisi kursor sampai akhir layar (hanya menu)

        // Cetak menu (tanpa mencetak ulang judul/info)
        for (int i = 0; i < (int)menuItems.size(); i++) {
            string text = menuItems[i];
            if (i == selected) {
                cout << COLOR_BLUE << " > " << text << COLOR_RESET << "\n";
            } else {
                if (i == (int)menuItems.size() - 1)
                    cout << COLOR_RED << "   " << text << COLOR_RESET << "\n";
                else
                    cout << COLOR_YELLOW << "   " << text << COLOR_RESET << "\n";
            }
        }

        int result = menuSelector(menuItems, selected);

        if (result < 0) {
            int indeksTerpilih = -result - 1;
            int aksi = aksiKode[indeksTerpilih];
            switch (aksi) {
                case 1: {
                    showKursor();
                    tambahHistory(indexAkun, c->judul, c->id);
                    tampilIsiCerpen(indexAkun, c);
                    hideKursor();
                    break;
                }
                case 2: {
                    showKursor();
                    toggleBookmark(indexAkun, c->judul);
                    hideKursor();
                    break;
                }
                case 0: {
                    showKursor();
                    return;
                }
            }
            break;
        } else {
            selected = result;
            // Lanjutkan loop, menu akan diperbarui dengan highlight baru
        }
    }
  }
}

void menuListCerpen(int indexAkun)
{
  while (true)
  {
    clearScreen();
    tampilHeader("DAFTAR CERPEN");

    if (jumlahCerpen == 0)
    {
      cout << COLOR_CYAN << "\n  Belum ada cerpen.\n" << COLOR_RESET;
      cout << "\n  Tekan Enter untuk kembali...";
      showKursor();
      cin.ignore();
      cin.get();
      return;
    }

    // Buat array pointer untuk navigasi
    vector<Cerpen*> items;
    for (int i = 0; i < jumlahCerpen; i++)
    {
      items.push_back(&daftarCerpen[i]);
    }
    items.push_back(nullptr); // untuk tombol Kembali

    int selected = 0;
    int total = items.size();

    hideKursor();
    while (true)
    {
      clearScreen();
      tampilHeader("DAFTAR CERPEN");
      cout << "\n";

      // Tampilkan daftar dengan highlight
      for (int i = 0; i < jumlahCerpen; i++)
      {
        if (i == selected)
        {
          cout << COLOR_GREEN << "  > " << COLOR_RESET;
          cout << COLOR_WHITE << "[" << items[i]->id << "] " << items[i]->judul << COLOR_RESET;
        }
        else
        {
          cout << "    [" << items[i]->id << "] " << items[i]->judul;
        }
        cout << COLOR_CYAN << " - " << items[i]->penulis << " (" << items[i]->genre << ")" << COLOR_RESET << "\n";
      }

      // Tombol Kembali
      if (selected == jumlahCerpen)
      {
        cout << COLOR_RED << "\n  > Kembali" << COLOR_RESET << "\n";
      }
      else
      {
        cout << COLOR_RED << "\n    Kembali" << COLOR_RESET << "\n";
      }

      cout << COLOR_CYAN << "\n  Gunakan panah atas/bawah, Enter untuk pilih\n" << COLOR_RESET;

      int ch = _getch();

      if (ch == 0 || ch == 224) // Tombol panah
      {
        ch = _getch();
        if (ch == 72) // Atas
          selected = (selected - 1 + total) % total;
        else if (ch == 80) // Bawah
          selected = (selected + 1) % total;
      }
      else if (ch == 13) // Enter
      {
        if (selected == jumlahCerpen) // Kembali
        {
          showKursor();
          return;
        }
        else // Pilih cerpen
        {
          showKursor();
          cerpenInfo(indexAkun, items[selected]);
          hideKursor();
          break; // Kembali ke list setelah lihat detail
        }
      }
    }
  }
}

// ─── Tambah Cerpen ───────────────────────────────────────────

void menuTambahCerpen(int indexAkun)
{
  showKursor();
  clearScreen();
  tampilHeader("TAMBAH CERPEN");

  string judul, genre, isi, waktu = getCurrentTimestamp();

    cout << "\n  " << COLOR_CYAN << "(ketik 0 untuk batal)\n"
       << COLOR_RESET;

  cout << "\n  Judul  : ";
  cin.sync();
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
    vector<string> menu = {
        "Lihat Semua Cerpen",
        "History",
        "Favorit",
        "Tambah Cerpen",
        "Logout"
    };

    int selected = 0;

    hideKursor();
    while (true)
    {
        clearScreen();

        // HEADER SELALU DI ATAS
        tampilHeader("MENU UTAMA");

        cout << COLOR_GREEN << "\n  Halo, "
             << daftarAkun[indexAkun].nama
             << " !\n  Selamat Datang di CerpenKita, mau baca apa hari ini?\n" << COLOR_RESET;

        // RENDER MENU
        for (int i = 0; i < menu.size(); i++)
        {
            string text = menu[i];

            // 🔵 kalau sedang dipilih
            if (i == selected)
            {
                cout << COLOR_BLUE << " > " << text << COLOR_RESET << "\n";
            }
            else
            {
                // 🔴 Logout khusus merah
                if (i == 4)
                    cout << COLOR_RED << "   " << text << COLOR_RESET << "\n";
                else
                    cout << COLOR_YELLOW << "   " << text << COLOR_RESET << "\n";
            }
        }

        int result = menuSelector(menu, selected);

        if (result < 0)
        {
            int choice = -result - 1;

            switch (choice)
            {
            case 0: menuListCerpen(indexAkun); break;
            case 1: menuHistory(indexAkun); break;
            case 2: menuFavorit(indexAkun); break;
            case 3: menuTambahCerpen(indexAkun); break;
            case 4: showKursor(); return;
            }
        }
        else
        {
             selected = result;
        }
    }
}

// ─── Register ────────────────────────────────────────────────

void menuRegister()
{
  showKursor();
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

  daftarAkun[jumlahAkun].nama = nama;
  daftarAkun[jumlahAkun].password = password;
  daftarAkun[jumlahAkun].jumlahRiwayat = 0;
  daftarAkun[jumlahAkun].jumlahBookmark = 0;
  jumlahAkun++;

  cout << COLOR_GREEN << "\n  Akun berhasil dibuat! Silakan login.\n"
       << COLOR_RESET;
  cout << "  Tekan Enter...";
  cin.get();
}

// ─── Login ───────────────────────────────────────────────────

void menuLogin()
{
  showKursor();
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
  // Bersihkan counter saja, array otomatis di-overwrite saat diisi ulang
  for (int i = 0; i < jumlahAkun; i++) {
    daftarAkun[i].jumlahRiwayat = 0;
    daftarAkun[i].jumlahBookmark = 0;
  }
  
  for (int i = 0; i < jumlahCerpen; i++) {
    daftarCerpen[i].jumlahKomentar = 0;
  }
  
  jumlahCerpen = 0;
}

// ─── Main ─────────────────────────────────────────────────────

int main()
{
    // Data dummy akun
    daftarAkun[0].nama = "hirsya";
    daftarAkun[0].password = "123";
    daftarAkun[0].jumlahRiwayat = 0;
    daftarAkun[0].jumlahBookmark = 0;
  
    daftarAkun[1].nama = "ridho";
    daftarAkun[1].password = "321";
    daftarAkun[1].jumlahRiwayat = 0;
    daftarAkun[1].jumlahBookmark = 0;
  
    jumlahAkun = 2;

  // Data dummy cerpen
  seedCerpen();

      vector<string> menu = {
      "[1] Login",
      "[2] Register",
      "[3] keluar"
    };
    int selected = 0;
    hideKursor();
    while (true)
    {
      clearScreen();
      tampilHeader("CERPENKITA");

        // RENDER MENU
        for (int i = 0; i < menu.size(); i++)
        {
            string text = menu[i];

            // 🔵 kalau sedang dipilih
            if (i == selected)
            {
                cout << COLOR_BLUE << " > " << text << COLOR_RESET << "\n";
            }
            else
            {
                // 🔴 Logout khusus merah
                if (i == 2)
                    cout << COLOR_RED << "   " << text << COLOR_RESET << "\n";
                else
                    cout << COLOR_YELLOW << "   " << text << COLOR_RESET << "\n";
            }
        }

        int result = menuSelector(menu, selected);

        if (result < 0)
        {
            int choice = -result - 1;

            switch (choice)
            {
            case 0: menuLogin();
                    break;
            case 1: menuRegister();
                    break;
            case 2: bersihkanMemori();
            showKursor();
            clearScreen();
            cout << COLOR_GREEN << "  Sampai jumpa!\n\n" << COLOR_RESET;
            return 0;
            default: break;
            }
        }
        else
        {
            selected = result;
        }
    }
}
