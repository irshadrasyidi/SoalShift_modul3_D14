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

## Soal 5
