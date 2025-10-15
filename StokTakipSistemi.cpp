#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits> // Gerekirse sayýsal sýnýrlarý kontrol etmek için

using namespace std;

// Ürün bilgilerini saklamak için struct yapýsý
struct Urun {
    int id;
    string ad;
    int stokMiktari;
    double fiyat;
};

const string DOSYA_ADI = "stok.txt";

// Fonksiyon prototipleri (önceden tanýmlama)
void urunEkle(vector<Urun>& stok);
void urunleriListele(const vector<Urun>& stok);
void urunSil(vector<Urun>& stok);
void urunGuncelle(vector<Urun>& stok);
int urunBul(const vector<Urun>& stok, int id);
void dosyayaKaydet(const vector<Urun>& stok);
void dosyadanYukle(vector<Urun>& stok);
void menuGoster();
// Ek fonksiyonlar: Kullanýcý giriþini almak için
int getIntegerInput();
double getDoubleInput();
string getStringInput();

int main() {
    vector<Urun> stok;
    dosyadanYukle(stok); // Program baþladýðýnda dosyadan stok verilerini yüklemek için

    int secim;
    do {
        menuGoster();
        secim = getIntegerInput(); // Kullanýcýdan tamsayý giriþi al
        switch (secim) {
            case 1:
                urunEkle(stok);
                break;
            case 2:
                urunleriListele(stok);
                break;
            case 3:
                urunSil(stok);
                break;
            case 4:
                urunGuncelle(stok);
                break;
            case 5:
                dosyayaKaydet(stok); // Program kapanmadan önce verileri dosyaya kaydetmek
                cout << "Programdan cikiliyor..." << endl;
                break;
            default:
                cout << "Gecersiz seçim. Lutfen tekrar deneyin." << endl;
        }
    } while (secim != 5);

    return 0;
}

// Menüyü görüntüleyen fonksiyon
void menuGoster() {
    cout << "\nStok Takip Sistemi Menusu" << endl;
    cout << "1. Urun Ekle" << endl;
    cout << "2. Urunleri Listele" << endl;
    cout << "3. Urun Sil" << endl;
    cout << "4. Urun Guncelle" << endl;
    cout << "5. Cikis" << endl;
    cout << "Seciminiz: ";
}

// Ürün ekleme fonksiyonu
void urunEkle(vector<Urun>& stok) {
    Urun yeniUrun;

    cout << "Urun ID'sini girin: ";
    yeniUrun.id = getIntegerInput(); // Kullanýcýdan  tamsayý al

     // ID'nin benzersiz olup olmadýðýný kontrol et
    if (urunBul(stok, yeniUrun.id) != -1) {
        cout << "Bu ID'ye sahip bir urun zaten var. Lutfen farkli bir ID girin." << endl;
        return; // Fonksiyondan çýk, ürün eklenmesin
    }
    cout << "Urun adini girin: ";
    yeniUrun.ad = getStringInput(); // Kullanýcýdan string al

    cout << "Stok miktarini girin: ";
    yeniUrun.stokMiktari = getIntegerInput(); // Kullanýcýdan tamsayý al

    cout << "Urun fiyatini girin: ";
    yeniUrun.fiyat = getDoubleInput(); // Kullanýcýdan double al

    stok.push_back(yeniUrun);
    cout << "Urun basariyla eklendi." << endl;
}

// Ürünleri listeleyen fonksiyon
void urunleriListele(const vector<Urun>& stok) {
    if (stok.empty()) {
        cout << "Stokta hic urun bulunmamaktadir." << endl;
        return;
    }
    cout << "\n--- Urun Listesi ---" << endl;
    for (size_t i = 0; i < stok.size(); ++i) {
        const Urun& urun = stok[i]; // Referansý al
        cout << "ID: " << urun.id << ", Ad: " << urun.ad << ", Stok: " << urun.stokMiktari << ", Fiyat: " << urun.fiyat << endl;
    }
}

// Ürün silme fonksiyonu
void urunSil(vector<Urun>& stok) {
    if (stok.empty()) {
        cout << "Silinecek urun bulunmamaktadir. Stok zaten bos." << endl;
        return;
    }
    cout << "Silmek istediginiz urunun ID'sini girin: ";
    int id = getIntegerInput(); // Kullanýcýdan tamsayý al

    int index = urunBul(stok, id); // urunBul fonksiyonunu kullan

    if (index != -1) {
        stok.erase(stok.begin() + index);
        cout << "Urun basariyla silindi." << endl;
    } else {
        cout << "Belirtilen ID'ye sahip bir ürün bulunamadi." << endl;
    }
}

// Ürün güncelleme fonksiyonu
void urunGuncelle(vector<Urun>& stok) {
    if (stok.empty()) {
        cout << "Guncellenecek urun bulunmamaktadir. Stok zaten bos." << endl;
        return;
    }

    cout << "Guncellemek istediginiz urunun ID'sini girin: ";
    int id = getIntegerInput(); // Kullanýcýdan tamsayý al

    int index = urunBul(stok, id); // urunBul fonksiyonunu kullan

    if (index != -1) {
        Urun& guncellenecekUrun = stok[index]; // Daha okunabilir olmasý için referans kullan
        cout << "Yeni urun adini girin (Mevcut: " << guncellenecekUrun.ad << "): ";
        string yeniAd = getStringInput();
        if (!yeniAd.empty()) { // Kullanýcý yeni bir ad girdiyse güncelle
            guncellenecekUrun.ad = yeniAd;
        }

        cout << "Yeni stok miktarini girin (Mevcut: " << guncellenecekUrun.stokMiktari << "): ";
        int yeniStokMiktari = getIntegerInput();
        if (yeniStokMiktari != numeric_limits<int>::min()) { // Kullanýcý geçerli bir sayý girdiyse güncelle
             guncellenecekUrun.stokMiktari = yeniStokMiktari;
        }

        cout << "Yeni urun fiyatini girin (Mevcut: " << guncellenecekUrun.fiyat << "): ";
        double yeniFiyat = getDoubleInput();
        if (yeniFiyat != numeric_limits<double>::min()) { // Kullanýcý geçerli bir sayý girdiyse güncelle
            guncellenecekUrun.fiyat = yeniFiyat;
        }
        cout << "Urun basariyla guncellendi." << endl;
    } else {
        cout << "Belirtilen ID'ye sahip bir urun bulunamadi." << endl;
    }
}

// Ürün arama fonksiyonu (ID'ye göre)
int urunBul(const vector<Urun>& stok, int id) {
    for (size_t i = 0; i < stok.size(); ++i) {
        if (stok[i].id == id) {
            return i; // Ürünün bulunduðu index'i döndür
        }
    }
    return -1; // Ürün bulunamazsa -1 döndür
}

// Verileri dosyaya kaydetme fonksiyonu
void dosyayaKaydet(const vector<Urun>& stok) {
    ofstream dosya(DOSYA_ADI.c_str()); // DOSYA_ADI.c_str() kullanýyoruz
    if (dosya.is_open()) {
        for (const auto& urun : stok) {
            // Ürün bilgilerini dosyaya yazarken virgülle ayýr
            dosya << urun.id << "," << urun.ad << "," << urun.stokMiktari << "," << urun.fiyat << endl;
        }
        dosya.close();
        cout << "Veriler basariyla kaydedildi." << endl;
    } else {
        cerr << "Dosya acma hatasi!" << endl; // cerr hata mesajlarý için kullanýlýr
    }
}

// Verileri dosyadan yükleme fonksiyonu
void dosyadanYukle(vector<Urun>& stok) {
    ifstream dosya(DOSYA_ADI.c_str());
    if (dosya.is_open()) {
        string satir;
        while (getline(dosya, satir)) {
            stringstream ss(satir);
            string deger;
            Urun urun;

            // ID okuma
            getline(ss, deger, ',');
            try {
                urun.id = atoi(deger.c_str()); // stoi yerine atoi (C-style string)
            } catch (...) { // Tüm istisnalarý yakala (C++98 için daha genel)
                cerr << "Gecersiz ID formati: " << deger << " atlandi" << endl;
                continue;
            }

            // Ad okuma
            getline(ss, deger, ',');
            urun.ad = deger;

            // Stok Miktarý okuma
            getline(ss, deger, ',');
            try {
                urun.stokMiktari = atoi(deger.c_str()); // stoi yerine atoi
            } catch (...) {
                cerr << "Gecersiz stok miktari formati: " << deger << " atlandi" << endl;
                continue;
            }

            // Fiyat okuma
            getline(ss, deger, ',');
            try {
                urun.fiyat = atof(deger.c_str()); // stod yerine atof (C-style string)
            } catch (...) {
                cerr << "Gecersiz fiyat formati: " << deger << " atlandi" << endl;
                continue;
            }

            stok.push_back(urun);
        }
        dosya.close();
        cout << "Veriler basariyla yuklendi." << endl;
    } else {
        cout << "Dosya bulunamadi. Yeni bir stok dosyasi olusturulacak." << endl;
    }
}
// Kullanýcýdan tamsayý giriþi alma fonksiyonu
int getIntegerInput() {
    int sayi;
    while (!(cin >> sayi)) {
        cout << "Gecersiz giris. Lütfen bir tamsayi girin: ";
        cin.clear(); // Hata bayraðýný temizle
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tamponu temizle
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return sayi;
}

// Kullanýcýdan  ondalýklý sayý giriþi alma fonksiyonu
double getDoubleInput() {
    double sayi;
    while (!(cin >> sayi)) {
        cout << "Gecersiz giriþ. Lütfen bir sayi girin: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return sayi;
}

// Kullanýcýdan string giriþi alma fonksiyonu
string getStringInput() {
    string str;
    getline(cin, str);
    return str;
}
