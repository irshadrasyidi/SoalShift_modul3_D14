# SoalShift_modul3_D14

## Soal 1
- Pada soal 1, inputan diterima bersama-sama saat program mulai dijalankan, seperti ini
```
./faktorial 5 3 4
```
- Sehingga, pada fungsi main perlu dimodifikasi argumennya seperti ini
```
int main(int argc, char *argv[])
```
- `int argc` adalah jumlah argumen yang diterima, dan `char *argv[]` adalah untuk wadah disimpannya argumen-argumen yang diterima. Jika didapat inputan `./faktorial 5 3 4`, maka:
  - `int argc` = 4
  - `argv[0]` = `./faktorial`
  - `argv[1]` = `5`
  - `argv[2]` = `4`
  - `argv[3]` = `3`
- Setelah inputan diterima, kita perlu mengkonversi argumen bilangan yang diterima menjadi int karena sebelumnya masih berupa string, di sini hasilnya disimpan di `arr[i]`. Konversi menggunakan fungsi `strtol()` dengan basis 10 karena basis bilangan yang akan kita gunakan adalah bilangan basis 10
```
for(i = 0; i < argc - 1; i++){
		konvers = strtol(argv[i + 1], &ptr, 10);
		arr[i] = konvers;
}
```
- Karena output diminta untuk menampilkan dari kecil ke besar, maka `arr[]` akan disort terlebih dulu, di sini menggunakan bubblesort
- Setelah `arr[]` disort, selanjutnya siapkan fungsi algoritma penghitungan faktorial yang akan digunakan untuk pembuatan thread
```
void *faktorial(void* arg){
	unsigned long long hasil = 1;
	long f, awal;
	awal = (long)arg;
	for(f = (long)arg; f > 0; f--){
		hasil *= (unsigned long long)f;
	}
	printf("%ld! = %llu\n", awal, hasil);
	return NULL;
}
```
- Pembuatan thread, di sini menggunakan loop, dan argumen yang dipassing ke fungsi faktorialnya adalah array `arr[]` dengan index `i` sesuai pada urutan loop. Gunakan variabel `argc` pada loop untuk mengetahui berapa kali pembuatan thread yang sesuai
```
for(i = 0; i < argc - 1; i++){
		err = pthread_create(&(tid[i]), NULL, &faktorial, (void*)arr[i + 1]);
		pthread_join(tid[i], NULL);
}
```
- Perhatikan bahwa line untuk mengeluarkan output berada di dalam fungsi `faktorial()`, sehingga hasil faktorial otomatis keluar dan sudah terurut



## Soal 2
Tidak selesai
Bingung saat akan mengimplementasikan soal ke contoh codingan socket client/server pada modul, dan waktunya habis di ngotak-ngatik nomor 5

REVISI:
- Pada nomor 2, diperlukan 2 server dan 2 klien untuk menjalankan transaksinya
- Masing2 klien terhubung dengan masing2 servernya dengan port yang dispesifikasikan dan antara pembeli dan penjual harus berbeda
  - Pada server dan klien pembeli digunakan `PORT 8088`
  - Pada server dan klien penjual digunakan `PORT 9099`
- Lalu, supaya stok antara penjual dan pembeli sama, maka digunakan shared memory dengan key yang sama, dan diletakkan di kedua file server
```
key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stok = shmat(shmid, NULL, 0);
```
- Klien beli dan Klien jual, hanya bertugas menerima inputan string "beli" untuk klien pembeli dan "tambah" untuk klien penjual
```
while(1){
	scanf("%s", buy);
	if(strcmp(buy, "beli") == 0){
		send(sock, buy, strlen(buy), 0);
		valread = read(sock, buffer, 1024);
		printf("%s\n", buffer);
	}
	else{
		printf("string tidak dikenal\n");
	}

	memset(buy, 0, 50);
	memset(buffer, 0, 1024);
}
```
- Di atas adalah contoh pada klien pembeli, fungsi `send()` akan mengirim string `buy` ke server
- Lalu fungsi `read()` akan membaca balasan dari server yang menggunakan wadah string `buffer`
- Pada server pembeli, dibuat sebuah fungsi thread untuk transaksi beli, dengan membaca string `buffer` kiriman dari klien
```
while((valread = read(new_socket ,buffer, 1024)) > 0){
		if(!strcmp(buffer, "beli")){
			if(*stok <= 0){
				strcpy(buy, "transaksi gagal");
				send(new_socket, buy, strlen(buy), 0);	
			}
			else{	
				*stok -= 1;
				strcpy(buy, "transaksi berhasil");
				send(new_socket, buy, strlen(buy), 0);
			}
		}
		memset(buy, 0, 50);
		memset(buffer, 0, 1024);
}
```
- Jika benar stringnya adalah "beli", maka dicek lagi jika stoknya kosong, maka `transaksi gagal`, tapi jika stok ada, maka stok
akan berkurang 1
- Pemanggilan thread akan dilakukan di dalam sebuah loop, tapi dapat dipastikan klien yang diterima hanya 1 karena fungsi `accept` dijalankan di luar loop, dan jika thread sudah dijalankan, ada integer `cekkon` sebagai flag supaya thread hanya berjalan 1 kali saja
- Jangan lupa untuk mengirim pesan string bahwa transaksi gagal/berhasil ke klien menggunakan fungsi `send()` pada masing2 `if`
- Pada server penjual, ada 2 fungsi thread yang dijalankan yaitu `cetakstok` dan `jual`
- Pada `cetakstok`, fungsi akan mengeprint stok setiap 3 detik
```
void* cetakstok(void *arg){
	while(1){
		printf("stok barang = %d\n", *stok);
		sleep(3);
	}
}
```
- Pada `jual`, fungsi ini bekerja untuk mengecek apakah ada string "tambah" yang diterima dari klien
```
void* jual(void* sock){
	int new_socket = *(int*)sock;
	char buffer[1024] = {0};
	int valread;
	while((valread = read(new_socket ,buffer, 1024)) > 0){
		if(!strcmp(buffer, "tambah")){
			*stok += 1;
		}
		memset(buffer, 0, 1024);
	}
}
```
- Jika benar, maka stok akan ditambah 1
- Untuk pemanggilan threadnya, sama seperti pada server pembeli, yaitu menggunakan flag `cekkon` sehingga dapat dipastika klien yang terhubung hanya 1

## Soal 3
- Di nomor 3, digunakan 3 thread:
  - Untuk mendisable fitur Agmal bangun
  - Untuk mendisable fitur Iraj tidur
  - Untuk menghitung waktu dalam second sejak dimulainya program
- Thread pen-disable-an menggunakan pengecekan flag/counter berapa kali fitur dipanggil
```
void* agmal_bangun(void *arg){
	while(1){
		if(f_iraj >= 3){
			mulai_dis_agmal = waktu;
			dis_agmal = 1;
			f_iraj = 0;
		}
	}
}

void* iraj_tidur(void *arg){
	while(1){
		if(f_agmal >= 3){
			mulai_dis_iraj = waktu;
			dis_iraj = 1;
			f_agmal = 0;
		}
	}
}
```
- Untuk disable Agmal bangun, maka dicek apakah iraj dipanggil sudah 3 kali atau lebih. Jika benar maka waktu mulai disablenya ditandai waktu pada momen tersebut `mulai_dis_agmal = waktu;`, variabel trigger-disable-nya diset 1 `dis_agmal = 1;`, dan counter/flag iraj diset kembali ke 0 `f_iraj = 0;`. Begitu pula sebaliknya (fungsi untuk disable Iraj tidur)
- thread untuk penghitungan waktu cukup sederhana, dengan sebuah variabel integer yang diincrement 1 setiap selesai sleep 1 detik
```
void* basetime(void *arg){
	while(1){
		sleep(1);
		waktu++;
	}
}
```
- Ketiga thread ini dipanggil pada awal main
```
pthread_create(&(tid[0]), NULL, &agmal_bangun, NULL);
pthread_create(&(tid[1]), NULL, &iraj_tidur, NULL);
pthread_create(&(tid[2]), NULL, &basetime, NULL);
```
- Setelah dipanggil, maka masuk ke loop `while(1)` untuk penginputan terus menerus. Di bagian awal, adalah pengecekan status wakeup-nya agmal dan spirit-nya iraj sebagai tanda berhentinya program
```
if(wakeup >= 100){
		printf("Agmal Terbangung,mereka bangun pagi dan berolahraga\n");
		exit(0);
	}
	if(spirit <= 0){
		printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
		exit(0);
}
```
- Selanjutnya ada 3 `if` pengecekan inputan:
  - `s` : lihat status wakeup dan spirit
  ```
  if(!strcmp(action, "s")){
	printf("Agmal WakeUp_Status = %d\n", wakeup);
	printf("Iraj Spirit_Status = %d\n", spirit);
  }
  ```
  - `b`: panggil fitur Agmal Ayo Bangun
  - `t`: panggil fitur Iraj Ayo Tidur
- Pada 2 pemanggilan fitur ini ada pengecekan jika fitur itu terdisable atau tidak
```
if(waktu - mulai_dis_agmal <= 10 && mulai_dis_agmal != 0){
	while(waktu - mulai_dis_agmal <= 10){
		printf("Agmal Ayo Bangun disabled 10 s\n");
		sleep(1);
	}
	dis_agmal = 0;
	//continue;
}
else if(dis_agmal == 0){
	f_agmal++;
	wakeup += 15;
}
```
- Pengecekan menggunakan 2 variabel penunjuk waktu yang diolah oleh thread-thread yang telah dibuat
- Jika range waktu masih berada dalam 10 detik, maka masuk ke `while` yang mengeluarkan notif bahwa fitur masih terdisable
- Setelah `while` selesai, maka tandanya disable sudah berakhir, sehingga variabel tanda disable sedang aktif diset jadi 0 `dis_agmal = 0;`
- Lalu di `else if` nya adalah cek apakah variabel disable sedang menyala atau tidak `else if(dis_agmal == 0)`, jika tidak maka, fitur pengubahan status bisa dijalankan `f_agmal++; wakeup += 15;`
- Pengecekan antara `b` dan `s` sama, hanya saja variabelnya yang ditukar-tukar antara milik iraj dan agmal.



## Soal 4
- Pada soal 4, ada 3 thread yang akan dipanggil:
  - `ps` : thread ini menjalankan perintah untuk menampilkan 10 proses teratas dan disimpan ke folder dan file .txt yang diinginkan (1 atau 2, tergantung argumen pembuatan thread)
  ```
  ps -aux | head -11 | tail -10 > ~/Documents/FolderProses%d/SimpanProses%d.txt
  ```
  - `kompres` : thread ini menjalankan perintah untuk mengompres file .txt pada thread sebelumnya ke sebuah file .zip yang diinginkan (1 atau 2, tergantung argumen pembuatan thread) dan sekaligus menghapus file .txt sebelumnya
  ```
  zip -m -j -q ~/Documents/FolderProses%d/KompresProses%d.zip ~/Documents/FolderProses%d/SimpanProses%d.txt
  ```
  - `unzip` : thread ini menjalankan perintah untuk mengekstrak file .zip yang diinginkan (1 atau 2, tergantung argumen pembuatan thread) yang pada thread sebelumnya sudah dibuat
  ```
  unzip -q ~/Documents/FolderProses%d/KompresProses%d.zip -d ~/Documents/FolderProses%d/
  ```
- Pemanggilan thread cukup dilakukan berurutan dan ditutup dengan `pthread_join`, dengan menyelipkan sleep(15) disela-sela pemanggilan thread ke-2 dan ke-3, karena pengekstrakan dilakukan 15 detik setelah dibuat .zip-nya
```
pthread_create(&tid[0], NULL, &kompres, (void*)&file1);
	pthread_create(&tid[1], NULL, &kompres, (void*)&file2);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
	sleep(15);
	
	pthread_create(&tid[0], NULL, &unzip, (void*)&file1);
	pthread_create(&tid[1], NULL, &unzip, (void*)&file2);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
```
- Pada saat setiap pemanggilan thread, argumen dimasukkan sebuah integer sebagai pembeda untuk proses 1 dan 2
```
int* file1 = (int*)1;
int* file2 = (int*)2;
pthread_create(&tid[0], NULL, &unzip, (void*)&file1);
pthread_create(&tid[1], NULL, &unzip, (void*)&file2);
```


## Soal 5
Tidak selesai
Buntu di cara inputan yang otomatis (tanpa tekan enter) dan system clear yang digabungkan dengan saat menerima inputan, waktu dicoba outputnya jadi aneh dan saat digunakan system clear, inputan yang dimasukkan tidak terbaca, sehingga terhambat saat akan melanjutkan ke pengerjaan dan testing fitur2nya
REVISI:
- Pada soal 5, diminta ada 3 scene yang merupakan `standbymode`, `battlemode`, dan `shopmode` dengan tampilan sesuai pada soal
- Selain tampilan, di dalam 3 fungsi itu juga ada banyak `if` pengecekan inputan opsi dengan masing2 di dalamnya adalah action jika opsi tersebut dipilih
- Pengambilan inputan opsi menggunakan sebuah fungsi `kbhit()` yang memungkinkan user memberi inputan tanpa klik `enter`
- Pada fungsi 3 scene ini, dia akan langsung masuk ke loop `while(1)` dengan di bagian akhir ada `system("clear")` supaya tampilan pada terminal bersih
- Pada program ini dibuat juga 4 thread berikut:
  - `basetime` : sebuah fungsi penghitungan waktu secara manual menggunakan sebuah counter yang diincrement dan di-sleep 1 detik
  ```
  i = 1;
	while(1) {
		i++;
		sleep(1);
  }
  ```
  - `eat` : fungsi thread yang akan mengurangi hunger status setiap 10 detik
  ```
  while(1){
	while(status == 0){}
	hunger -= 5;
	sleep(3);
	//sleep(10);
  }
  ```
  - `bath` : fungsi thread yang akan mengurangi hygiene status setiap 30 detik
  ```
  while(1){
	while(status == 0){}
	hygiene -= 10;
	sleep(6);
	//sleep(30);
  }
  ```
  - `regen` : fungsi thread yang akan menambah health status setiap 10 detik
  ```
  while(1){
	while(status == 0){}
	health += 5;
	sleep(3);
	//sleep(10);
  }
  ```
  (Note: nama fungsi tidak berkorelasi dengan fungsi sebenarnya, hanya penamaannya saja)
  (Note: detik sleep yang diaktifkan bukan yang diminta pada soal, supaya waktu dicek tidak perlu menunggu terlalu lama)
- Pada awal program, diminta inputan string untuk nama monster
- Lalu, dipanggil 4 thread yang tadi sudah deklarasikan
- Setelah thread dipanggil, panggil fungsi `standbymode()` sebagai scene default program
- Pada program ini juga digunakan shared memory yang merupakan stok makanan toko untuk digunakan juga di program lain yaitu `shop`
```
key_t key = 1234;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stokmakantoko = shmat(shmid, NULL, 0);
```
- Berikut adalah action2 yang dijalankan jika monster ingin diberi makan (opsi 1)
```
if(inp == '1') {
	if(stokmakan > 0){
		if(hunger >= 200){
			printf("Full\n");
		}
		else if(stokmakan -15 < 0){
			hunger += stokmakan;
			stokmakan = 0;
		}
		else{
			stokmakan -= 15;
			hunger += 15;
		}

		if(hunger > 200) {
			stokmakan += (hunger-200);
			hunger = 200;
		}
	}
	else{
		printf("stok habis\n");
}
```
- Lalu, jika yang diinginkan opsi untuk mandi, maka actionnya sebagai berikut
```
else if(inp == '2'){
	if(cooldownbath == 0 || i - cooldownbath > 20) {
		cooldownbath = i;
		printf("Bath is ready\n");
		hygiene += 30;
	}
	else{
		printf("Bath will be ready in %ds\n", cooldownbath + 20 - i);
	}
}
```
- Dapat dilihat pengecekan cooldownnya dengan memanfaatkan counter `i` yang ada di thread `basetime`
- Lalu, untuk opsi 3, user akan masuk ke battlemode, dengan men-set dulu health dari enemy-nya jadi 100 dulu, lalu set `status` mejadi 0 supaya thread2 `eat`, `bath`, dan `regen` sementara berhenti
```
else if(inp == '3'){
	enemyhealth = 100;
	system("clear");
	status = 0;

	battlemode();
}
```
- Pada `battlemode()` ada 2 opsi: Attack dan Run
- Jika attack, maka dilakukan action2 berikut, sesuai yang diminta pada soal
```
if(inp == '1'){
	enemyhealth -= 20;
	health -= 20;
	if(health <= 0){
		printf("%s kalah hiya3x\n", nama);
		sleep(1);
		system("clear");
		exit(-1);
	}

	if(enemyhealth <= 0){
		printf("%s menang\n", nama);
		sleep(1);
		system("clear");
		status = 1;

		standbymode();
	}
} 
```
- Jika health kita habis, maka kalah dan langsung exit. Jika health musuh yang habis, maka menang dan kembali ke `standbymode()`
- Jika opsi 2 (run), maka akan langsung kembali ke `standbymode()`
```
else if(inp == '2'){
	printf("kaboor\n");
	sleep(1);
	system("clear");

	standbymode();
}
```
- Untuk shopmode, ada 2 opsi: Buy dan Back
- Untuk Buy, maka akan dicek dulu stok makan toko, apakah ada atau tidak. Jika ada maka stok makan toko berkurang, dan stok makan monster bertambah. Jika stok makan toko tidak ada, maka transaksi gagal
```
if(inp == '1'){
	if(*stokmakantoko > 0) {
		printf("Transaksi berhasil\n");
		*stokmakantoko = *stokmakantoko - 1;
		stokmakan++;
	}
	else printf("Transaksi gagal\n");
} 
```
- Untuk back, maka langsung kembali ke `standbymode()`
```
else if(inp == '2'){
	printf("kembali\n");
	sleep(1);
	system("clear");
	status = 1;

	standbymode();
}
```
- Pada program toko-nya, dibuat juga fungsi penerima input `kbhit()` supaya input tanpa enter
- Lalu, ada juga thread `inshopmode` yang akan masuk ke loop `while(1)` yang berfungsi hanya tampilan pada terminal
- Pada main, thread dipanggil terlebih dahulu, supaya bisa menampilkan tampilannya shop
- Lalu, ada pengecekan inputannya opsi yang dipilih: 1 untuk menambah stok toko, 2 untuk exit
- Jika opsi 1 dipilih maka stok makan toko ditambah
```
if(inp == '1'){
	*stokmakantoko = *stokmakantoko + 1;
}
```
- Ingat, variabel stok makan toko sudah digunakan pada shared memory sehingga dapat dilihat juga pada program utama
