#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits> // Gerekirse say�sal s�n�rlar� kontrol etmek i�in

using namespace std;

// �r�n bilgilerini saklamak i�in struct yap�s�
struct Urun {
    int id;
    string ad;
    int stokMiktari;
    double fiyat;
};

const string DOSYA_ADI = "stok.txt";

// Fonksiyon prototipleri (�nceden tan�mlama)
void urunEkle(vector<Urun>& stok);
void urunleriListele(const vector<Urun>& stok);
void urunSil(vector<Urun>& stok);
void urunGuncelle(vector<Urun>& stok);
int urunBul(const vector<Urun>& stok, int id);
void dosyayaKaydet(const vector<Urun>& stok);
void dosyadanYukle(vector<Urun>& stok);
void menuGoster();
// Ek fonksiyonlar: Kullan�c� giri�ini almak i�in
int getIntegerInput();
double getDoubleInput();
string getStringInput();

int main() {
    vector<Urun> stok;
    dosyadanYukle(stok); // Program ba�lad���nda dosyadan stok verilerini y�klemek i�in

    int secim;
    do {
        menuGoster();
        secim = getIntegerInput(); // Kullan�c�dan tamsay� giri�i al
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
                dosyayaKaydet(stok); // Program kapanmadan �nce verileri dosyaya kaydetmek
                cout << "Programdan cikiliyor..." << endl;
                break;
            default:
                cout << "Gecersiz se�im. Lutfen tekrar deneyin." << endl;
        }
    } while (secim != 5);

    return 0;
}

// Men�y� g�r�nt�leyen fonksiyon
void menuGoster() {
    cout << "\nStok Takip Sistemi Menusu" << endl;
    cout << "1. Urun Ekle" << endl;
    cout << "2. Urunleri Listele" << endl;
    cout << "3. Urun Sil" << endl;
    cout << "4. Urun Guncelle" << endl;
    cout << "5. Cikis" << endl;
    cout << "Seciminiz: ";
}

// �r�n ekleme fonksiyonu
void urunEkle(vector<Urun>& stok) {
    Urun yeniUrun;

    cout << "Urun ID'sini girin: ";
    yeniUrun.id = getIntegerInput(); // Kullan�c�dan  tamsay� al

     // ID'nin benzersiz olup olmad���n� kontrol et
    if (urunBul(stok, yeniUrun.id) != -1) {
        cout << "Bu ID'ye sahip bir urun zaten var. Lutfen farkli bir ID girin." << endl;
        return; // Fonksiyondan ��k, �r�n eklenmesin
    }
    cout << "Urun adini girin: ";
    yeniUrun.ad = getStringInput(); // Kullan�c�dan string al

    cout << "Stok miktarini girin: ";
    yeniUrun.stokMiktari = getIntegerInput(); // Kullan�c�dan tamsay� al

    cout << "Urun fiyatini girin: ";
    yeniUrun.fiyat = getDoubleInput(); // Kullan�c�dan double al

    stok.push_back(yeniUrun);
    cout << "Urun basariyla eklendi." << endl;
}

// �r�nleri listeleyen fonksiyon
void urunleriListele(const vector<Urun>& stok) {
    if (stok.empty()) {
        cout << "Stokta hic urun bulunmamaktadir." << endl;
        return;
    }
    cout << "\n--- Urun Listesi ---" << endl;
    for (size_t i = 0; i < stok.size(); ++i) {
        const Urun& urun = stok[i]; // Referans� al
        cout << "ID: " << urun.id << ", Ad: " << urun.ad << ", Stok: " << urun.stokMiktari << ", Fiyat: " << urun.fiyat << endl;
    }
}

// �r�n silme fonksiyonu
void urunSil(vector<Urun>& stok) {
    if (stok.empty()) {
        cout << "Silinecek urun bulunmamaktadir. Stok zaten bos." << endl;
        return;
    }
    cout << "Silmek istediginiz urunun ID'sini girin: ";
    int id = getIntegerInput(); // Kullan�c�dan tamsay� al

    int index = urunBul(stok, id); // urunBul fonksiyonunu kullan

    if (index != -1) {
        stok.erase(stok.begin() + index);
        cout << "Urun basariyla silindi." << endl;
    } else {
        cout << "Belirtilen ID'ye sahip bir �r�n bulunamadi." << endl;
    }
}

// �r�n g�ncelleme fonksiyonu
void urunGuncelle(vector<Urun>& stok) {
    if (stok.empty()) {
        cout << "Guncellenecek urun bulunmamaktadir. Stok zaten bos." << endl;
        return;
    }

    cout << "Guncellemek istediginiz urunun ID'sini girin: ";
    int id = getIntegerInput(); // Kullan�c�dan tamsay� al

    int index = urunBul(stok, id); // urunBul fonksiyonunu kullan

    if (index != -1) {
        Urun& guncellenecekUrun = stok[index]; // Daha okunabilir olmas� i�in referans kullan
        cout << "Yeni urun adini girin (Mevcut: " << guncellenecekUrun.ad << "): ";
        string yeniAd = getStringInput();
        if (!yeniAd.empty()) { // Kullan�c� yeni bir ad girdiyse g�ncelle
            guncellenecekUrun.ad = yeniAd;
        }

        cout << "Yeni stok miktarini girin (Mevcut: " << guncellenecekUrun.stokMiktari << "): ";
        int yeniStokMiktari = getIntegerInput();
        if (yeniStokMiktari != numeric_limits<int>::min()) { // Kullan�c� ge�erli bir say� girdiyse g�ncelle
             guncellenecekUrun.stokMiktari = yeniStokMiktari;
        }

        cout << "Yeni urun fiyatini girin (Mevcut: " << guncellenecekUrun.fiyat << "): ";
        double yeniFiyat = getDoubleInput();
        if (yeniFiyat != numeric_limits<double>::min()) { // Kullan�c� ge�erli bir say� girdiyse g�ncelle
            guncellenecekUrun.fiyat = yeniFiyat;
        }
        cout << "Urun basariyla guncellendi." << endl;
    } else {
        cout << "Belirtilen ID'ye sahip bir urun bulunamadi." << endl;
    }
}

// �r�n arama fonksiyonu (ID'ye g�re)
int urunBul(const vector<Urun>& stok, int id) {
    for (size_t i = 0; i < stok.size(); ++i) {
        if (stok[i].id == id) {
            return i; // �r�n�n bulundu�u index'i d�nd�r
        }
    }
    return -1; // �r�n bulunamazsa -1 d�nd�r
}

// Verileri dosyaya kaydetme fonksiyonu
void dosyayaKaydet(const vector<Urun>& stok) {
    ofstream dosya(DOSYA_ADI.c_str()); // DOSYA_ADI.c_str() kullan�yoruz
    if (dosya.is_open()) {
        for (const auto& urun : stok) {
            // �r�n bilgilerini dosyaya yazarken virg�lle ay�r
            dosya << urun.id << "," << urun.ad << "," << urun.stokMiktari << "," << urun.fiyat << endl;
        }
        dosya.close();
        cout << "Veriler basariyla kaydedildi." << endl;
    } else {
        cerr << "Dosya acma hatasi!" << endl; // cerr hata mesajlar� i�in kullan�l�r
    }
}

// Verileri dosyadan y�kleme fonksiyonu
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
            } catch (...) { // T�m istisnalar� yakala (C++98 i�in daha genel)
                cerr << "Gecersiz ID formati: " << deger << " atlandi" << endl;
                continue;
            }

            // Ad okuma
            getline(ss, deger, ',');
            urun.ad = deger;

            // Stok Miktar� okuma
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
// Kullan�c�dan tamsay� giri�i alma fonksiyonu
int getIntegerInput() {
    int sayi;
    while (!(cin >> sayi)) {
        cout << "Gecersiz giris. L�tfen bir tamsayi girin: ";
        cin.clear(); // Hata bayra��n� temizle
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tamponu temizle
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return sayi;
}

// Kullan�c�dan  ondal�kl� say� giri�i alma fonksiyonu
double getDoubleInput() {
    double sayi;
    while (!(cin >> sayi)) {
        cout << "Gecersiz giri�. L�tfen bir sayi girin: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return sayi;
}

// Kullan�c�dan string giri�i alma fonksiyonu
string getStringInput() {
    string str;
    getline(cin, str);
    return str;
}
