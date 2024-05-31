# Homework 1
Ad Soyad: Önder Görmez  
Öğrenci No: 21501035

# Problem 1: CSRF Defenses (40p)

## 1 - A Şıkkı
### Soru
Ders kapsamında Cross Site Request Forgery (CSRF) saldırılarının oturum yönetimi için sadece cookie’lerden yararlanan sitelere yönelik olduğu işlenmişti. Böyle bir siteye gerçekleştirilecek bir CSRF atağını kısaca adım adım açıklayınız.

### Cevap
**CSRF Kısa Tanım**: Bir web uygulamasına, o web uygulamasının güvendiği (authenticate ettiği) bir kullanıcı olarak request göndermek anlamına gelmektedir.  

**CSRF Atağı Adımları**:
* Kullanıcı login olduktan sonra gönderdiği diğer requestlerde sürekli olarak username ve password girmek zorunda kalmaması için session bazlı cookie ler kullanılır.
* Kötü niyetli bir site veya kullanıcı bu cookie yi ele geçirirse, authenticate edilmiş, onaylanmış bir kullanıcı gibi request gönderebilir.
* Alıcı web uygulaması bu requesti, gerçek kullanıcı tarafından gönderilmiş bir request olarak algılar ve işler. Böylelikle attack gerçekleşmiş olur.
* Genel olarak bu attack cookie lerin kötüye kullanılması (sömürülmesi) ile yapılmaktadır.

**Çıkarım**:  
Yukardaki açıklamalardan yola çıkarak cookie based authentication ın;
* Side effect leri olan requestler için (bir kullanıcıdan başka bir kullanıcıya para transferi gibi)
* Genel olarak karşı tarafta bir değişikliğe neden olan requestler için (POST, PUT, DELETE gibi)  

yeterli olmadığını söyleyebiliriz.  

**Savunma Teknikleri**:
* Referer Validation (Header da referer bilgisinin kontrol edilmesi)
* Secret Validation Token (Formlarda hidden value olarak bir token eklenmesi)
* Custom HTTP Header (Header da özel bir bilginin kontrol edilmesi)
* SameSite Cookies (Cookie lerin sadece aynı origin de çalışmasını sağlamak)

## 1 - B Şıkkı
### Soru
CSRF’e karşı genel savunma tekniklerinden biri DOM içerisine, örneğin gizli bir form elemanı olarak, bir token yerleştirmek ve sunucuya POST ile gönderilen isteklerde cookie yanı sıra bu token değerini göndermek şeklindedir. Bu tekniğin (a) şıkkındaki saldırıyı niye engellemeyeceğini kısaca açıklayınız.

### Cevap

Bu gizli token değeri tek başına saldırıyı engelleyemez. Engellemek için daha akıllı bir yöntem olan random ve unique token oluşturulmalıdır (c şıkkındaki konu). Detaylar aşağıda;  

DOM'a tipi hidden bir secret token (**csrf_token**) aşağıdakine benzer şekilde eklenir.
* İsmi değişebilir (implementation defined), değerinin ne olduğu önemlidir.

 ```html
<form action="https://example.com/api/transfer" method="POST">
    <input type="hidden" name="fixed_csrf_token" value="0ea93d4a26aa821f">
    
    <input type="text" name="to" value="1000$">

    <button type="submit">Transfer</button>
</form>
 ```

Attacker csrf_token ın değerini bilmeden, server tarafına token eksik bir şekilde formu submit ederse;
* Server tarafında bu token kontrolü fail olacağı için request işlenmeyecektir.
* Tıpkı referer alanının boş olması kontrolü gibi, bu token alanı da boş olursa request işlenmeyecektir.

Fakat saldırgan, out of band denilen şekilde;
* Bu sayfayı **browser olmadan yükleyebilir ve value değerini** alabilir.
* Bu value değerini kullanarak, formu doldurup, server a gönderebilir.
* Böylelikle başarılı bir saldırı gerçekleşmiş olacaktır.
* Saldırının başarılı olmasının nedeni token ın value sunun random ve unique bir şekilde değişmemesidir.

## 1 - C Şıkkı
### Soru
Diğer bir yaklaşım da bahsi geçen token değerinin web server tarafından yollanan http cevaplarının (response) içine rasgele üretilerek konulması ve bir sonraki HTTP isteğinde bu token değerinin beklenmesi olabilir. Bu yaklaşım CSRF saldırılarını engeller mi? Eğer engeller ise nasıl engellediğini açıklayınız. Eğer engellemiyorsa nasıl saldırı yapılabileceğini açıklayınız.

### Cevap
Bu yaklaşım CSRF saldırılarını engeller. Bunun nedeni form her yüklendiğinde farklı random ve unique token değerleri oluşturulmasıdır.  

Form Load 1;
  ```html
<form action="https://example.com/api/transfer" method="POST">
    ...
    <input type="hidden" name="random_unique_csrf_token" value="253ad839cee7aff8">
    ...
</form>
 ```

Form Load 2;
  ```html
<form action="https://example.com/api/transfer" method="POST">
    ...
    <input type="hidden" name="random_unique_csrf_token" value="49786b1cb64a062d">
    ...
</form>
 ```

* **Token** ın değeri her request için **random bir şekilde değişmeli ve unique olmalıdır**.
* Sayfanın bir parçası olarak session-specific bir token (random ve unique) gönderilmelidir.
  * **Same Origin Policy (SOP) nedeniyle**, kullanıcının açtığı diğer bir sekmedeki token değerine attacker erişemez.
  * Attacker ın yüklediği kötü niyetli javascript kodlarının bu token a erişmesi browser tarafından engellenir.
  * Attacker yeni bir sekmede veya browser harici bir uygulama ile formu load ederse, burada oluşan token değeri kullanıcının session ındaki token değeri ile uyuşmayacaktır. Bu durum da request in işlenmemesine neden olacaktır. 

## 1 - D Şıkkı
### Soru
Bir diğer yaklaşım ise (c) şıkkındaki rasgele token yerine sabit bir token değerinin kullanılması olabilir. Bu durum CSRF saldırılarını engeller mi? Eğer engeller ise nasıl engellediğini açıklayınız. Eğer engellemiyorsa nasıl saldırı yapılabileceğini açıklayınız.

### Cevap
Sabit token yöntemi CSRF saldırılarını engelleyemez.

Form Load (Her request için aynı token değeri);
 ```html
<form action="https://example.com/api/transfer" method="POST">
    ...
    <input type="hidden" name="fixed_csrf_token" value="0ea93d4a26aa821f">
    ...
</form>
 ```

Random token yerine sabit token kullanılması çok tehlikeli bir durumdur.
* Bu durumda saldırganın işi çok daha kolaylaşır.
* Çünkü saldırgan, bu sabit token değerini öğrendiği anda, her request te bu token değerini kullanarak, server a istek gönderebilir.
* Token doğru olacağı için server tarafından request işlenir ve saldırı başarılı olur.
* SOP nedeniyle kullanıcının gönderdiği token değerine saldırgan erişemez.

 Fakat saldırgan, out of band denilen şekilde;
* Bu sayfayı **browser olmadan yükleyebilir ve sabit value değerini** alabilir.
* Değeri kullanarak, formu doldurup, server a gönderebilir.
* Böylelikle başarılı bir saldırı gerçekleşmiş olacaktır.
* Saldırının başarılı olmasının nedeni **token ın value sunun** random ve unique bir şekilde **değişmemesidir**.

## 1 - E Şıkkı
### Soru
Same-Origin Policy yaklaşımı cookie ve token kullanımı ile yapılan CSRF saldırılarına karşı savunmada nasıl bir önem taşımaktadır? Açıklayınız.

### Cevap
Diğer şıklarda da SOP'nin saldırıyı nasıl engellediği açıklanmıştı.  
Kısaca özetlemek gerekirse, **Same Origin Policy (SOP) sayesinde**;
  * Attacker ın ele geçirdiği bir sitenin sekmesi, kullanıcının açtığı diğer bir sekmedeki token değerine erişemez.
  * Attacker ın yüklediği kötü niyetli javascript kodlarının token a erişmesi browser tarafından engellenir.
  * Attacker yeni bir sekmede veya browser harici bir uygulama ile formu load ederse, burada oluşan token değeri kullanıcının session ındaki token değeri ile uyuşmayacaktır. Bu durum da request in işlenmemesine neden olacaktır.



<div style="page-break-after: always;"></div>

# Problem 2: Stack Canaries (30p)

## 2 - A Şıkkı
### Soru
Derste konuştuğumuz gibi GCC ile bir C programı derlendiğinde eğer -fstack-protector bayrağı kullanılırsa her stack frame içerisine koruma amaçlı bir kanarya (canary) değeri yerleştirilmektedir. Stack smashing (stackde yer alan geri dönüş adresinin ezilmesi) saldırısına açık olan ve bunu göstermek için komut satırından giriş kabul eden kısa bir C programı yazın ve -fstack-protector bayrağı ile derlenmiş olsa bile ilgili saldırının yapılabildiğini gösterin. (İpucu olarak yazacağınız programda stack bölgesinde içinde bir pointer ve bir de string olan bir struct yapısının olduğundan yola çıkabilirsiniz. Bu yapı içerisindeki alanların birbirlerinin ardı sıra geldiğini ve ilk alanın bellekte ikinci alandan daha düşük adreste saklandığını kabul edebilirsiniz. Bu durumda string değişkeninin taşırılmasını sağlayabilirseniz pointer alanını da ezebilirsiniz ve kanarya ile korumaya rağmen saldırının gerçekleşebilmesi saldırganın stackteki bu alanları ezebilmesi üzerine kurulu olmalı.)

### Cevap
Projenin oluşturulması ve derlenmesi aşağıdaki gibidir.

* İsterleri sağlayan bir C programı yazılmıştır.
```C
#include <stdio.h>
#include <string.h>

typedef struct Vulnerable {
    char *ptr;
    char buffer[16];
} Vulnerable;

int main(int argc, char **argv) {
    Vulnerable v;
    v.ptr = NULL;
    printf("ptr: %p\n", v.ptr);

    if (argc > 1) {
        strcpy(v.buffer, argv[1]);
        printf("Input was: %s\n", v.buffer);
        printf("ptr: %p\n", v.ptr);
    }
    return 0;
}
```

* Yukarıdaki kod dan anlaşılacağı üzere 16 byte lık buffer taşırılarak ptr ın değeri değiştirilmeye çalışılmıştır.
* Derlemek için kullanılan meson.build dosyası aşağıdaki gibidir.

```
project('BLM5102_Computer_Systems_and_Network_Security', 'c', 'cpp',
  version : '0.1',
  default_options : ['c_std=c89', 'cpp_std=c++17', 'warning_level=3'])

add_global_arguments('-fno-stack-protector', language : 'c')

message('meson.build file find successfully in: ' + meson.current_source_dir())

executable('stack_canaries_app',
           'main.c',
           install : false)
```

* Aşağıda detayları verilen toolchain kullanılarak derlenmiştir.
```sh
ondergormez@Onders-MacBook-Pro stack_canaries_app % meson setup build_dir                
# The Meson build system
# Version: 1.4.1
# Source dir: /Users/ondergormez/Repos/BLM5102_Computer_Systems_and_Network_Security/02-Homeworks/Homework1/stack_canaries_app
# Build dir: /Users/ondergormez/Repos/BLM5102_Computer_Systems_and_Network_Security/02-Homeworks/Homework1/stack_canaries_app/build_dir
# Build type: native build
# Project name: BLM5102_Computer_Systems_and_Network_Security
# Project version: 0.1
# C compiler for the host machine: ccache cc (clang 15.0.0 "Apple clang version 15.0.0 (clang-1500.1.0.2.5)")
# C linker for the host machine: cc ld64 1022.1
# C++ compiler for the host machine: ccache c++ (clang 15.0.0 "Apple clang version 15.0.0 (clang-1500.1.0.2.5)")
# C++ linker for the host machine: c++ ld64 1022.1
# Host machine cpu family: aarch64
# Host machine cpu: aarch64
# Message: meson.build file find successfully in: /Users/ondergormez/Repos/BLM5102_Computer_Systems_and_Network_Security/02-Homeworks/Homework1/stack_canaries_app
# Build targets in project: 1
# 
# Found ninja-1.12.1 at /opt/homebrew/bin/ninja
ondergormez@Onders-MacBook-Pro stack_canaries_app % 

ondergormez@Onders-MacBook-Pro stack_canaries_app % cd build_dir 
ondergormez@Onders-MacBook-Pro build_dir % meson compile
# INFO: autodetecting backend as ninja
# INFO: calculating backend command to run: /opt/homebrew/bin/ninja
# [1/2] Compiling C object stack_canaries_app.p/main.c.o
# [2/2] Linking target stack_canaries_app
ondergormez@Onders-MacBook-Pro build_dir % 
```

* Program normal bir input ile çalıştırıldığında sorunsuz çalıştığı gözlemlenmiştir.

```sh
ondergormez@Onders-MacBook-Pro build_dir % ./stack_canaries_app hello
# ptr: 0x0
# Input was: hello
# ptr: 0x0
ondergormez@Onders-MacBook-Pro build_dir % 

# Sınır Değer
ondergormez@Onders-MacBook-Pro build_dir % ./stack_canaries_app 123456789012345 
# ptr: 0x0
# Input was: 123456789012345
# ptr: 0x0
ondergormez@Onders-MacBook-Pro build_dir % 
````

* Fakat 16 byte taşıran bir input ile çalıştırıldığında programın çöktüğü gözlemlenmiştir. Programın çöktüğü için aslında stack smashing saldırısını gerçekleştiremedim.

```sh
ondergormez@Onders-MacBook-Pro build_dir % ./stack_canaries_app 1234567890123456
# ptr: 0x0
# zsh: trace trap  ./stack_canaries_app 1234567890123456
ondergormez@Onders-MacBook-Pro build_dir % 
```

**Önemli Not**: GNU Debugger (GDB) ile debug edilmek istendi fakat Apple M3 işlemcili Macbook Pro da yüklenemedi.
```sh
ondergormez@Onders-MacBook-Pro ~ % brew install gdb
# gdb: The x86_64 architecture is required for this software.
# Error: gdb: An unsatisfied requirement failed this build.
ondergormez@Onders-MacBook-Pro ~ %
```

**Önemli Not**: UC San Diego üniversitesinin Stack Smashing ile ilgili faydalı bir kaynağıda vardı. İncelendi. Fakat GDB yüklenemediği için debug edilemedi. [[3]](#[3])

## 2 - B Şıkkı
### Soru
İşletim sisteminin bütün stack alanlarını non-executable olarak işaretlediği bir durumda stack smashing saldırısı yapılabilir mi? Cevabınızı kısaca açıklayınız.

### Cevap
**Non-executable Stack**: Stack alanının sadece veri depolamak için kullanılmasını sağlayan bir güvenlik önlemidir. Bu sayede stack alanına yazılan kodlar çalıştırılamaz.
* İşletim sisteminin stack alanlarını non-executable olarak işaretlemesi, stack alanında bir shell açılmasını engeller.



<div style="page-break-after: always;"></div>

# Problem 3: Same Origin Policy (30p)
### Soru
Dönem içerisinde DOM kapsamında same-origin policy açısından kaynak (origin) tanımının (protocol, domain, port) üçlüsünden oluştuğu işlenmişti. DOM kapsamında same-origin policy tanımının sadece ve sadece domain ile yapılması durumunda ortaya çıkacak problemleri ve saldırganlar açısından oluşacak fırsatları belirtiniz ve açıklayınız. Yaptığınız açıklamalardan yola çıkarak standart tanımda oluşmayacak ancak gevşetilmiş yeni tanımdan oluşacak ve saldırganlar tarafından kullanılabilecek net bir örnek veriniz.

### Cevap
**Same Origin Policy Kısa Tanım**: Birbirinden farklı kaynakların, birbirlerinin datalarına erişimlerinin kısıtlanmasını (izole edilmesi) sağlayan temel yapılardan biridir.
* **Confidentiality (Gizlilik)**: abc.com sitesindeki bir script, xyz.com sitesinin datalarına erişememelidir. Verinin gizliliği sağlanmalıdır.
* **Integrity (Bütünlük)**: abc.com sitesindeki bir script, xyz.com sitesinin datalarını değiştirememelidir. Verinin bütünlüğü korunmalıdır.
* Unix te bulunan Security Model ile benzerdir.

**Subjects (Who?)**
* Unix: Users, processes
* Burada origins -> scheme://domain:port şeklinde tanımlanır.

**Objects (What?)**
* Unix: Files, directories, devices, sockets
* Burada DOM tree, DOM storage, cookies, etc. bulunur.

**Access Operaitons (How?)**
* Unix: Read, write, execute
* Burada  GET, POST, PUT gibi yöntemlerle yapılır.
* attacker.com içerisinde bir frame olarak bank.com u koysak bile, kullanıcı tarafından bank.com için girilen login ve password verilerine erişemeyiz. Çünkü SOP buna izin vermez.

**Kısıtlamaların biraz daha gevşetilmesi gereken durumlar olabilir.**
* Cross-Origin Resource Sharing (CORS)
* Super Domain

* Sadece ve sadece domain ile same-origin policy tanımı yapılmasının amacı aynı domain e ait farklı sub domainlerin birbirlerinin login bilgilerine erişerek sürekli login olmadan data çekebilmesini, kullanıcının domain içerisinde bulunan farklı sub domainler arasında rahatça gezinmesini sağlamaktır.
* Buna **domain relaxation** denir.

```
CDN: Content Delivery Network
cdn.facebook.com --> facebook.com a kendisini dönüştürebilir.
profile.facebook.com --> facebook.com a kendisini dönüştürebilir.
```
* Fakat bu durumda bazı güvenlik açıkları da ortaya çıkmaktadır.

**Ortaya çıkacak problemler ve saldırganlar açısından oluşacak fırsatlar nelerdir?**

* DNS sunucusu ele geçirilirse, isimlerin çözümü kontrol edilmeye başlanırsa sorunlu bir durum oluşabilir.
* Aynı domainde bulunan diğer web sayfalarının cookielerine erişebilir ve bu yolla başka kullanıcılar gibi davranabilir.

**Standart tanımda oluşmayacak fakat relaxed tanımda oluşacak ve saldırganlar tarafından kullanılabilecek net bir örnek veriniz.**

Üst Domain: https://yildiz.edu.tr  
Alt Domain: https://obs.yildiz.edu.tr/

https://obs.yildiz.edu.tr/ üzerinde bir zaafiyet varsa ve bu site ele geçirildiyse, aşağıdaki durumlar oluşabilir.
* Alt domain üst domainden (https://yildiz.edu.tr) cookie çalabilir.
* Lokal de yildiz.edu.tr ye login olduysanız, bu login bilgilerindeki cookie ler çalınarak sizin adınıza https://yildiz.edu.tr adresinde işlem yapılabilir.

Bu durumların önüne geçmek için her iki domainde de **document.domain** flag i set edilmek durumundadır.  



<div style="page-break-after: always;"></div>

# Useful Resources and References
- [1] <a name="[1]"> [Open Worldwide Application Security Project (OWASP) - Cross Site Request Forgery (CSRF)](https://owasp.org/www-community/attacks/csrf)
- [2] <a name="[2]"> [Meson Build System - Tutorial](https://mesonbuild.com/Tutorial)
- [3] <a name="[3]"> [UC San Diego - Stack Smashing](https://cseweb.ucsd.edu/~dstefan/cse127-winter20/notes/bufferoverflow/)
