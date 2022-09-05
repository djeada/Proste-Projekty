# Proste-Projekty
Kolekcja pomysłów na projekty dla początkujących programistów wraz z pełnymi specyfikacjami oraz przykładowymi implementacjami.

## Spis Treści
<!--ts-->
- [Jak zacząć projekt?](#Jak-zacząć-projekt) 
- [Tak zwane dobre praktyki](#Tak-zwane-dobre-praktyki)
- [Lista projektów](#Lista-projektów)
- [Dodatkowe materiały](#Dodatkowe-materiały) 
<!--te-->

## Jak zacząć projekt?

1. Upewnij się, że znasz podstawy wybranego języka programowania. Nie ma znaczenia, jaki język wybierzesz. Ważne jest to, byś dobrze opanował wszystkie fundamentalne koncepcje i mechanizmy (zmienne, warunki, pętle, funkcje, tablice i napisy). Korzystaj z dobrodziejstw internetu. Poszukaj materiałów, które pasują do twojego stylu uczenia się i ucz się z nich. Zadania treningowe znajdziesz pod tym <a href="https://github.com/djeada/Nauka-Programowania">linkiem</a>.
1. Wybierz temat, który cię interesuje. Szukaj inspiracji w swoim życiu codziennym. Może potrzebujesz zautomatyzować wysyłanie e-maili. A może masz ochotę zagrać w jakąś grę, której nikt jeszcze nie stworzył. Nie ma nic złego w czerpaniu pomysłów od innych programistów (chodzi o samą ideę, przy podkradaniu kodu musisz najpierw sprawdzić licencję). Strona <a href="https://github.com">GitHub</a> to dobre miejsce do tego typu poszukiwań. Przyjrzy się projektom stworzonym przez innych początkujących programistów i zastanów się, czy coś podobnego mogłoby cię zainteresować. Nie kopiuj kodu, na razie skup się na samym pomyśle.
1. Pamiętaj im oryginalniejszy pomysł, tym lepiej. Świat widział już milion tetrisów, więc jest okazja, by wykazać się pomysłowością. Nie przejmuj się jednak jeśli zupełnie nie przychodzi ci do głowy żaden nowatorski pomysł. Od tego masz to repozytorium, by w najgorszym wypadku powielić jeden z popularnych tematów. Nie kopiuj jednak wszystkie, dodaj kilka funkcjonalności od siebie i rozbuduj projekt, na którym bazujesz.
1. Oceń realność ukończenia wybranego przez ciebie projektu. Czy wiesz w ogóle jak zacząć? Jeśli nie, to być może lepiej będzie jak, zdecydujesz się na coś mniej ambitnego. Jeśli wiesz mniej więcej, od czego należałoby rozpocząć kodowanie, to następnie spróbuj przygotować ogólną listę kroków, potrzebnych do ukończenia projektu. Spisz je (na papierze lub w wersji elektronicznej). To bardzo ważne.
1. Jeśli jeszcze tego nie zrobiłeś, pobierz środowisko programistyczne odpowiednie dla wybranego przez ciebie języka. W większości przypadków <code>VS Code</code> to dobra opcja. Pełną kontrolę nad wersjami języka oraz bibliotek masz jedynie gdy kodujesz lokalnie, na swoim komputerze.
1. Przygotuj szkielet projektu (zdefiniuj, jakie pliki będą ci potrzebne). Ten krok jest mocno uzależniony od języka programowania, jaki wybrałeś. Przykładowo jeśli tworzysz aplikację webową w czystym JavaScript to prawdopodobnie, będziesz potrzebował minimum trzy pliki: *index.html*, *index.js* i *index.css*. Być może jednak chcesz napisać skrypt w Pythonie, wtedy *main.py* oraz *requirements.txt* mogą być wszystkim, czego na początku potrzebujesz. Dla języków kompilowanych prawdopodobnie będziesz jeszcze musiał napisać skrypt z regułami budowania aplikacji (taki skrypt definiuje jakie pliki wynikowe powinny powstać w wyniku kompilacji, jak połączyć je z zewnętrznymi bibliotekami itd.). 
1. Niezależnie od języka programowanie będziesz potrzebować dokumentacji. Na samym początku wystarczy plik *README.md*, opisz cel projektu, wszystkie zależności, które są potrzebne do zainstalowania, by uruchomić aplikację oraz krótka informacja o obsłudze interfejsu graficznego (może być również krótkie demo w formacie GIF). Na późniejszym etapie projektu mogą cię zainteresować cię generatory dokumentacji jak <a href ="https://www.sphinx-doc.org/en/master/">SPHINX</a> lub <a href="https://doxygen.nl/">Doxygen</a>.
1. Umieść wszystkie przygotowane pliki w obrębie repozytorium Git.
1. Jeśli dotarłeś tak daleko, to przyszedł czas na chwilę relaksu. Weź głęboki oddech i rozluźnij się. 
1. Teraz przyszedł czas na gwóźdź programu, implementację wcześniej przygotowanej listy kroków. Zanim jednak zaczniesz pisać kod, zastanów się jeszcze czy nie można niektórych punktów rozbić na mniejsze oraz, czy nie zabrakło czegoś istotnego. Wraz z rozwojem projektu będziesz jeszcze niejednokrotnie modyfikował tę listę.
1. Implementuj funkcjonalności jedna po drugiej. Po zaimplementowaniu każdej z nich testuj ręcznie swój program. Nie zakładaj, że wiesz, co zrobią napisane przez ciebie wiersze kodu. Programowanie lubi płatać figle i nigdy nie jest do końca tak, jakby się wydawało na początku. Poza testami ręcznymi byłoby miło pomyśleć również o testach automatycznych. Każdy język programowania ma przynajmniej kilka bibliotek/frameworków do testowania programów. Znajdź coś odpowiedniego dla ciebie. Dzięki testom automatycznym szybko sprawdzisz, czy nowe funkcjonalności nie psują działania tych, które zaimplementowałeś wcześniej. Również refaktoryzacja będzie o niebo prostsza.
1. Każda funkcjonalność powinna być osobno zapisywana jako commit w repozytorium. Dzięki temu twój postęp będzie archiwizowany i zawsze będziesz mieć możliwość wrócenia do ostatniej działającej wersji kodu. Dodatkowo jeśli swoje repozytorium umieścisz również na zewnętrznym serwerze (jak np. GitHub) będziesz miał zawsze dostępne kopie  zapasowe twojej pracy.
1. Gdy utkniesz, pytaj o pomoc wujka Google. Umiejętność znajdowania odpowiedzi na techniczne pytania jest niezwykle ważna dla każdego programisty. Staraj się dokładnie opisać swój problem. Jeśli nie znajdziesz rozwiązania twojego problemu, to musisz spróbować poszukać rozwiązania dla problemu ogólniejszego, a następnie przenieść je na twój problem. Choć w przypadku prostych projektów to mało prawdopodobne to są niszowe problemy, dla których nie ma rozwiązania na stronach typu <a href="https://stackoverflow.com/">Stack Overflow</a>. Trzeba będzie wtedy sięgnąć do dokumentacji i ruszyć głową.
1. Wypuść swój projekt na światło dzienne. Upublicznij kod w serwisie <a href="https://github.com">GitHub</a>. Dla aplikacji desktopowej możesz pomyśleć o przygotowaniu instalatora. Jeśli natomiast pracowałeś nad aplikacją webową, to możesz spróbować uruchomić ją na zewnętrznym serwerze za pomocą jednego z darmowych serwisów jak <a href="https://devcenter.heroku.com/">Heroku</a>.
1. Poinformuj mnie o swoim projekcie!

## Tak zwane dobre praktyki

Dobre praktyki to reguły pisania programów (niemalże) niezależne od języka programowania. 

### Obietnice dobrych praktyk

Co takiego dają nam tak zwane dobre praktyki?

* Kod staje się czytelniejszy.
* Łatwiej wprowadzać modyfikacje do kodu.
* Minimalizujemy objętość pisanego kodu.
* Możliwa jest praca w grupie.
* Program jest testowalny.

### Organizacja projektu

* Git jest twoim przyjacielem. Pracuj małymi krokami. Często zapisuj (commit) stan swojej pracy opisując przy tym naniesione zmiany. Jeśli pracujesz w grupie rozgałęzienie (<a href="https://en.wikipedia.org/wiki/Branching_(version_control)">feature branches</a>) projektu może mieć sens. Inną strategią jest regularna integracja nawet drobnych zmian z główną gałęzią (master) repozytorium (<a href="https://en.wikipedia.org/wiki/Continuous_integration">trunk-based development</a>).
* Czytaj już napisany kod. Jeśli uważasz, że coś da radę zrobić prościej bądź lepiej to spróbuj poprawić aktualny kod. Nie bój się wprowadzać zmian. Testy jednostkowe zabezpieczą cię przed wprowadzeniem destruktywnych zmian.
* Na bieżąco buduj dokumentacje. Dodawaj komentarze do tworzonych funkcji i klas. Gdy napotkasz trudności, zanotuj je i umieść krótkie streszczenie w *README.md*. 
* Dbaj o schludność i czytelność kodu. 
* Nieformatowany kod jest nieczytelny. Wybierz styl formatowania i trzymaj się go. Istnieją również setki narzędzi do automatycznego formatowania kodu. Używaj ich.
* Dziel kod między pliki, a pliki między foldery/moduły/paczki. Nazwy plików oraz folderów powinny być proste i sugestywne.
* Rozdzielaj zadania między małe funkcje i klasy (jeśli programujesz w języku obiektowym).
* Nie kopiuj żywcem kodu z internetu. Oczywiście jak najbardziej możesz, a nawet powinieneś szukać odpowiedzi na swoje pytania w internecie, gdyż wiele problemów, które napotkasz, zostało już wielokrotnie rozwiązanych. Należy jednak zaznaczyć, że fragmenty kodu, które znajdziesz, prawie nigdy nie będą idealnie pasować do twojego kodu. Za nim będziesz mógł ich użyć w swoim projekcie, będziesz musiał dopasować znaleziony kod do twojego kodu. W najlepszym wypadku niczego nie będziesz musiał przeklejać, wystarczy, że zrozumiesz zasadę działania znalezionego rozwiązania i samemu napiszesz analogiczne rozwiązanie.
* Uważaj na martwy kod. Nie twórz zmiennych, funkcji i klas, których nigdy nie użyjesz.
* Nie ignoruj ostrzeżeń kompilatora.

### Zmienne

* Nadawaj zmiennym nazwy, które coś znaczą. Nikt nie wie, co to jest <code>a</code> ani <code>xob</code>. Nie przesadzaj jednak. Jeśli tworzysz zmienną <code>sumaZarobkowWszystkichPracownikowFirmy</code>, tylko po to, by użyć jej raz bądź dwa razy to może wydałoby tę nazwę skrócić.
* Trzymaj się jednej konwencji nazewnictwa. Nie mieszaj <code>snake_case</code> z <code>camelCase</code>.
* Nie twórz zbędnych zmiennych, jeśli jakaś informacja jest już przechowywana w już istniejących zmiennych, to nie twórz dla niej nowych zmiennych.
* Nie zmieniaj nagle znaczenia zmiennych. Jeśli zmienna <code>suma</code> przechowywała jakąś sumę, to nie wykorzystuj jej do przechowywania średniej.
* Unikaj zmiennych globalnych. Bardzo szybko stracisz nad nimi kontrolę.
* Deklaruj zmienne jak najbliżej miejsca ich użycia.

### Warunki

* Nie komplikuj warunków. Jeśli chcesz jednocześnie sprawdzić kilka rzeczy i warunki łączysz przy pomocy operatorów logicznych, to lepiej wydeleguj to zadanie do osobnej funkcji.
* Unikaj zagnieżdżania warunków. Lepiej używać klauzuli ochronnych.
* Unikaj umieszczania wielu warunków jednego pod drugim. Lepiej rozdzielić ich zadania na osobne funkcje.
* Nie przyrównuj wyniku wywołania funkcji zwracającej typ logiczny (prawda lub fałsz) do wartości logicznych w warunkach. 

### Funkcje

* Używaj czasowników do nazywania funkcji.
* Każda funkcja powinna mieć jasno określony cel. Dodaj testy jednostkowe, by sprawdzić, czy cel ten został zrealizowany. 
* Nie używaj nazw, które wprowadzają czytelnika w błąd. Jeśli twoja funkcja nazywa się <code>czyPierwsza()</code>, to nie używaj jej do niczego innego poza sprawdzeniem, czy podana liczba jest liczbą pierwszą.
* Nie powtarzaj się (zasada DRY). Jeśli zauważyłeś, że twoim kodzie powtarza się kilkukrotnie ta sama kombinacja komend to warto je przenieść do osobnej funkcji
* Pamiętaj nierzadko im krótsza funkcja, tym lepsze. Nie rozciągaj definicji funkcji na dziesiątki wierszy. Nie ma jednak jednej liczby, która byłaby absolutnym maksimum liczby wierszy w funkcji.
* Funkcja powinny ukrywać wszelkie detale swojej implementacji. Przykładowo, gdy naciskam na przycisk na pilocie telewizyjnym, nie mam pojęcia, jak działa elektronika, z którą wchodzę w interakcje, wiem tylko jakiego efektu powinienem się spodziewać. Tak samo działają dobre funkcje. Złe funkcje natomiast poprzez niejasność swojej nazwy bądź skomplikowaną kombinację argumentów wymaga od nas zrozumienia implementacji do poprawnego jej użycia.
* Przy wywoływaniu funkcji przekazuj referencje do obiektów tam, gdzie niepotrzebna jest kopia. W wielu językach programowania (jak Python) jest to zachowanie domyślne, jednak mimo wszystko warto być świadomym tego faktu.    
* Dbaj o spójne przekazywanie danych między funkcjami.
* Pamiętaj o przypadkach granicznych. Jeśli twoja funkcja spełnia cel, do którego została stworzona to, jest to dopiero połowa sukcesu. Zastanów się, czy klient twojej funkcji ma możliwość przekazania do niej danych, dla których funkcja nie zwróci poprawnego wyniku lub gorzej, pojawi się błąd, który zatrzyma działanie całego programu. Nawet jeśli prawdopodobieństwo wystąpienia takiego przypadku jest niskie, to musisz i tak zadbać o to by funkcja umiała sobie z nim poradzić. 
* Argumenty typu logicznego są niepożądane. Lepiej podzielić funkcję dwie mniejsze, jedna dla sytuacji, gdy wartość argumentu to prawda, a druga  dla sytuacji, gdy wartość argumentu to fałsz.
* Nie ma ścisłego limitu na ilość argumentów funkcji, ale im więcej argumentów tym łatwiej jest się w nich pogubić. Dla funkcji potrzebujących wielu argumentów często wiele z nich ma zbliżony cel i łatwo da się je pogrupować w klasy/struktury.  
* Najlepsze funkcje to takie, które nie mają efektów ubocznych. To znaczy, że wywołanie funkcji nie zmienia niczego w stanie programu poza samą funkcją.

### Klasy

* Używaj rzeczowników do nazywania klas.
* Klasy powinny ukrywać pewną złożoność. Interfejs, zależności oraz skutki uboczne użycia klasy powinny być niewielkie w stosunku do złożoności ukrywanych przez klasę. Zła klasa to taka, której użycie jest trudniejsze niż bezpośrednia praca na danych, które przechowuje.
* Klasy mają za zadanie ukrywać dane oraz udostępnić interfejs do operowania na tych danych. W podejściu proceduralnym funkcje nie ukrywają danych. Nie mieszaj filozofii programowania. Używaj klas do celu, do jakiego zostały stworzone.
* Nie ulegaj pokusie łączenia wszystkich potrzebnych ci danych w jedną klasę. Grupuj dane w małe jednostki. Inaczej utrzymanie takiego kodu szybko stanie się trudniejsze niż złamanie szyfru enigmy.
* Nie twórz klas, które nie są potrzebne, tylko po to, by mieć klasy (chyba że programujesz w języku gdzie, wszystko musi być zamknięte w klasie). Jeśli nie potrzebujesz klasy, to nie twórz jej. Czasami zwykła funkcja jest całkowicie wystarczająca.
* Uważaj na funkcje, które zmieniają stan obiektu. 
* Jeśli dwie funkcje w różnych klasach mają dokładnie to samo zadanie, to powinny się również tak samo nazywać. 
* Nie twórz klas, które składają się z pól, które czasami są puste. Użyj wspólnego interfejsu dla kilku klas.
* Nie twórz kilku różnych klas, mogących być jedną klasą, której obiekty różnią się jedynie wartością pola. Przykładowo zamiast klas: <code>CzerwonyPrzycisk</code>, <code>ZielonyPrzycisk</code> i <code>NiebieskiPrzycisk</code> lepiej mieć klasę <code>Przycisk</code> z polem kolor.
* Klasy są po to, by pod jedną nazwą zamknąć dane oraz transformacje dostępne dla tych danych. Dla jednorazowej modyfikacji użyj funkcji.

### Komentarze 

* Koniecznie umieść w kodzie komentarze służące do generowania dokumentacji publicznego API (docstrings).
* Nie dodawaj niepotrzebnych komentarzy! Zbędne komentarze to takie, które wyjaśniają znaczenie składni języka programowania, zamiast wyjaśniać zamierzenia autora. Komentarze są po to, by pokazać innym programistom, dlaczego dana decyzja została podjęta. Kod jest po to, by pokazać, jak została zaimplementowana. Przykładowo jeśli w kodzie zwiększamy zmienną indeks o 2, aby uwzględnić przesunięcie wprowadzone przez nagłówek, to należy to objaśnić w komentarzu, zamiast pisać, że zmienna zostaje powiększona.
* Najgorsze komentarze, to komentarze dezinformujące. Programista mógł celowo chcieć opisać jedynie część prawdy, ale niepoprawny komentarz może być też wynikiem braku aktualizacji przy wprowadzaniu zmian w kodzie.
* Akceptowalne są komentarze TODO z listą zadań do wykonania.

### Obsługa błędów

* Zalecana strategia obsługi błędów jest mocno uzależnione od języka. Większość współczesnych języków posiada mechanizm wyjątków, którego nie ma w starszych językach jak C. Wyjątki oraz sytuacje, w których ich użycie jest zalecane, różnią się jednak nawet wśród języków, które je implementują.
* Ogólnie sytuacja wyjątkowa to taka, z którą dana funkcja nie może sobie w żaden sensowny sposób poradzić. Przykładowo jeśli implementujemy funkcje zwracającą wartość elementu list dla indeksu podanego jako parametr, to nie ma możliwości zwrócenia poprawnej wartości dla indeksów mniejszych od zera lub większych równych długości listy. W takiej sytuacji możemy wyrzucić wyjątek <code>IndexOutOfBound</code>.
* Użycie wyjątków jest generalnie preferowane nad zwracanie kodów błędu lub wartości NONE/NULL w przypadku niepowodzenia funkcji.
* Nie nadużywaj wyjątków.
* Przy wyrzucaniu wyjątków należy poinformować użytkownika o błędzie oraz o ile jest to możliwe wyjaśnić, dlaczego błąd się pojawił i gdzie dokładnie w kodzie jest problematyczne wywołanie funkcji.
* Jeśli wywołujesz funkcję, która może wyrzucić wyjątek, musisz opatrzyć ją mechanizmem obsługi danego wyjątku.
* Unikaj przekazywania do funkcji wartości NULL/NONE. Generalnie często prowadzi to do pojawienia się wyjątków jak <code>NullPointerException</code>.

### Struktury danych

* Dobierz odpowiednią strukturę danych do zadania.
* W wielu przypadkach zwykła tablica (lista) jest dostateczna i nie trzeba kombinować z innymi strukturami danych.
* Gdy chcesz użyć indeksów nie będących kolejnymi liczbami naturalnymi, użyj mapy (słownika).
* Zbiór zapewni unikalność elementów, ale utracimy informację o ich kolejności.

### Testy

* Dodaj testy jednostkowe dla każdej funkcji sprawdzające, czy wykonuje ona swoje zadanie.
* Unikaj powtórzeń w testach. Jedną rzecz musisz sprawdzić tylko raz.
* Testy są tak samo ważne jak kod produkcyjny.
* Dbaj o czystość testów.
* Testy jednostkowe nie powinny zajmować dużo czasu do wykonania. 
* Testy powinny być niezależne od siebie.
* Testy powinny być niezależne od środowiska, w którym są uruchamiane.
* Nie używaj funkcji <code>assert</code> do sprawdzania warunków w czasie wykonywania programu.

## Lista projektów

Projekty nie zostały uporządkowane względem żadnego kryterium.

### Szubienica

Gra polegająca na odgadywaniu słów. Komputer losuje słowo z wcześniej przygotowanej listy słów. Na ekranie wyświetlone zostaje n kresek, gdzie n to liczba liter, z których składa się słowo. Gracz ma x żyć. W każdym ruchu gracz podaje jedną literę. Jeśli odgadł jedną z liter, z których składa się słowo, to poziome kreski odpowiadające danej litery zostają w nią zamienione. W przeciwnym razie liczba x zmniejszana jest o 1. Gracz wygrywa, jeśli odgadnie wszystkie litery przed wyzerowaniem się licznika.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![hangman](https://user-images.githubusercontent.com/37275728/188334893-d1b25bd0-eda8-4053-b8ff-d38a52d72461.gif)< | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/hangman)**
 
</div>

### Szyfr Cezara

Głównym elementem interfejsu graficznego jest pole tekstowe, gdzie użytkownik wpisuje bądź przekleja tekst. Następnie ma on do wyboru jedną z dwóch opcji, szyfrowanie bądź odszyfrowanie. Dla obu opcji należy również podać wartość klucza. Po wypełnieniu wszystkich pól wyświetlany jest zmieniony tekst. W tle używany jest algorytm szyfru Cezara.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![caesar cipher](https://user-images.githubusercontent.com/37275728/188400957-b9590fdb-defe-4c77-a820-5bebde2f6450.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" /> | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/caesar_cipher)**
 
</div>

### Kalkulator

Prosty interfejs graficzny, który pozwala na wykonywanie różnych operacji matematycznych.

#### Linki

<div align="center">
 
Screenshot | Technologie | Link 
---|---|---
<img width="2000"/>![calculator](https://user-images.githubusercontent.com/37275728/188334898-ece55e10-0577-4c31-8e28-ab78739de2c4.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/calculator)**
 
</div>

### Lista zadań

Aplikacja pozwala na wyświetlenie, edycję i usuwanie zadań z listy. Zadania mogą być przechowywane w pliku tekstowym lub w bazie danych.

#### Linki

### Pogoda

Informacje o aktualnej temperaturze, pogodzie, prędkości wiatru, ciśnieniu atmosferycznym oraz wschodzie i zachodzie słońca dla danej lokacji. 

#### Linki

### Stoper

Aplikacja do odmierzania czasu. Użytkownik ma możliwość włączenia oraz zatrzymania stopera. Czas wyświetlany jest z dokładnością do milisekund.

#### Linki

### Statki

Gracz ma możliwość ustawienia swoich statków na planszy. Pozycje statków komputera są losowane. W każdej turze gracz próbuje odgadnąć pozycję komputera, a komputer wybiera losową pozycję na planszy gracza. Wygrywa ten, kto pierwszy zatopi wszystkie statki przeciwnika.

#### Linki

### Kółko i krzyżyk

Gra w kółko i krzyżyk. Dostępne są dwa tryby gry: gra dla dwóch graczy oraz gra z komputerem. Zwycięża ten, kto jako ustawił w linii (poziomo, pionowo lub na ukos) trzy kółka lub trzy krzyżyki. Jeśli wszystkie pola zostały zajęte i linia nie została utworzona, to mamy remis.

#### Linki

### Saper

Gra polega na wybieraniu pól na planszy. Na danym polu może być ukryta mina. Jeśli wybrano pole, na którym nie ma miny, to wyświetlona zostaje liczba min, które bezpośrednio stykają się z danym polem (od zera do ośmiu). Odkrycie pola z miną oznacza przegraną. Zwyciężyć można poprzez odkrycie wszystkich bezpiecznych pól.

#### Linki
 
<div align="center">

Screenshot | Technologie | Link 
---|---|---
<img width="2000"/>![minesweeper](https://user-images.githubusercontent.com/37275728/188334904-6544ac1c-839f-40af-a4e0-1d064a13c3f5.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/minesweeper)**
 
</div>

### 2048

Gracz przesuwa na planszy kwadraty z numerami będącymi kolejnymi potęgami dwójki. Po każdym ruchu pojawia się nowy kwadrat z numerem 2 lub 4. Gracz może łączyć kwadraty z tym samym numerem, wynikiem jest kwadrat o numerze równym sumie numerów kwadratów składowych. Gracz zwycięża, jeśli udało mu się otrzymać kwadrat o numerze 2048.

#### Linki

### Apokalipsa Zombie

Celem jest przetrwanie jak największej liczby fal zombie. Z każdą nową falą przychodzi coraz więcej i coraz silniejszych zombie. Gracz ma możliwość zbierania bonusów dających dodatkowe życie, alternatywne bronie oraz zwiększające prędkość gracza.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![zombie_apocalypse](https://user-images.githubusercontent.com/37275728/188334905-179b94fd-eec2-44b8-a64f-fecdd6c6ea01.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/zombie_apocalypse)**
 
</div>

### Piętnastka

Na planszy o wymiarach 4x4 mamy losowo ułożonych 15 kwadratów z numerami od 1 do 15 oraz jedno puste pole. Zadaniem gracza jest uporządkowanie kwadratów, tak by były ułożone w kolejności rosnącej.

#### Linki
 
<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![fifteen_puzzle](https://user-images.githubusercontent.com/37275728/188334915-1d7bd7b0-0a7e-4118-97e7-b12572b198e7.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/fifteen_puzzle)**
 
</div>

### Kości

Gra turowa dla przynajmniej dwóch graczy. Wszystkie akceptowalne kombinacje kości wraz z liczbą punktów za nie przyznawanych znajdują się w tabeli gry, a każdy gracz może użyć daną kombinację dokładnie raz. W każdej turze gracz rzuca maksymalnie trzykrotnie pięcioma kośćmi. Jeśli kości utworzyły jedną z pożądanych kombinacji, to informacja o tym zostaje zapisana w tabeli gry. W przeciwnym razie gracz musi wykreślić jedną z możliwych kombinacji ze swojej tabeli. Pod koniec gry sumowana jest liczba punktów z tabeli dla każdego z graczy. Wygrywa gracz z największą liczbą punktów.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![yahtzee](https://user-images.githubusercontent.com/37275728/188334918-95a7385c-3d10-4613-ae06-f0afafd0874e.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/yahtzee)**
 
</div>

### Kurka wodna

Kurki wlatują z prawej strony ekranu i wylatują z lewej strony. Zadaniem gracza jest ustrzelenie (poprzez kliknięcie myszą) każdej z kurek, zanim doleci ona na przeciwną stronę ekranu. Z każdym poziomem zmienia się tło i zwiększona zostaje liczba kurek.

#### Linki

### Wąż

Wąż zbudowany jest z n kwadratów i porusza się po prostokątnej planszy. Na planszy w równych odstępach czasowych pojawia się jedzenie. Połknięcie jedzenia przez węża zwiększa licznik punktów oraz powiększa go o jeden kwadrat. Gracz przegrywa, jeśli waz uderzy głową w część własnego ciała.

#### Linki

### Edytor tekstowy

pole tekstowe z mozliwoscia edytowania tekstu z klawiatury oraz wklejania tekstu. Opcja zapisywania oraz wczytywania. Podstawowe opcje formatowania, wielkosc czcionki, rodzina, pogrubienie, kursywa, podkreslenie.

#### Linki

### Obróbka grafiki

Program pozwala na wczytywanie oraz wyświetlanie plików graficznych w formatach PNG, oraz JPG. Dostępnych jest kilka funkcji do edycji obrazu (np. obróć, przytnij, rozciągnij). Oprócz okna z obrazem mamy również pasek z narzędziami (np. kolor, gumka, zaznacz, pędzel). Istnieje opcja eksportowania zmodyfikowanych obrazów.

#### Linki

### Git

Prosta implementacja podstawowych funkcjonalności systemu kontroli wersji. Istnieje możliwość zapisania aktualnego stanu projektu oraz wyświetlenia wszystkich poprzednich zapisanych stanów.

#### Linki

### Serwer HTTP
Serwer HTTP oczekuje dowolnego rodzaju żądań skierowanych pod URL *http://127.0.0.1:8000/*. Po odebraniu żądania skrypt wysyła odpowiedź z napisem "Witaj świecie". Bardziej zaawansowana wersja łączy serwer HTTP z dowolną bazą danych i daje użytkownikowi możliwość odczytania oraz modyfikowania danych przy pomocy przygotowanego API.

#### Linki

### Koszyk z zakupami

Zawartość koszyka jest w pełni edytowalna. Można do niego dodawać produkty, usuwać je, zmieniać ich ilość, wpisywać kody rabatowe i wrócić do kontynuowania zakupów. Produkty wyświetlane są w kolejności w jakiej zostały dodane. Każdy produkt posiada obrazek oraz informację o cenie.

#### Linki

### Zamawianie jedzenia

Głównym elementem aplikacji jest lista dostępnych dań wraz ze zdjęciami, ceną, opisem oraz tagami. Po zaznaczeniu dania musimy podać adres oraz wybrać formę płatności spośród PayPal, karta kredytowa lub gotówka przy odbiorze. Dodatkowo można zaimplementować opcję śledzenia kuriera.

#### Linki

### Komunikator internetowy

Aplikacja służąca do wysyłania tekstu między komputerami znajdującymi się w tej samej sieci LAN (w ambitniejszej wersji między dowolnymi urządzeniami z dostępem do internetu). Dodatkowo można dać możliwość tworzenia kont przez użytkowników, przesyłania plików oraz opcję wideorozmowy. 

#### Linki

## Dodatkowe materiały

Przykłady specyfikacji:

* https://en.wikipedia.org/wiki/Functional_specification
* https://web.eecs.utk.edu/~azh/blog/challengingprojects.html
* https://www.theodinproject.com/paths/full-stack-javascript
* https://people.kth.se/~johanmon/dse.html
* https://gist.github.com/MWins/41c6fec2122dd47fdfaca31924647499
* https://www.dj4e.com/project/00_overview.md
* https://www.ece.rutgers.edu/~marsic/books/SE/projects/
* http://cslibrary.stanford.edu/112/

Przykłady plików readme:

* https://github.com/alecortega/portfolio-template
* https://github.com/othneildrew/Best-README-Template

Przykłady pełnych implementacji programów:

* https://github.com/Python-World/python-mini-projects
* https://github.com/swharden/Csharp-Data-Visualization
* https://github.com/practical-tutorials/project-based-learning
* https://github.com/bradtraversy/50projects50days
