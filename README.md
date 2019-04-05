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

## Soal 4

## Soal 5
