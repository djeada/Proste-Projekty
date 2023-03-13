# Proste-Projekty
Kolekcja projektów dla początkujących programistów, zawierająca pełne specyfikacje oraz przykłady implementacji.

## Spis Treści
<!--ts-->
- [Jak zacząć projekt?](#Jak-zacząć-projekt) 
- [Tak zwane dobre praktyki](#Tak-zwane-dobre-praktyki)
- [Lista projektów](#Lista-projektów)
- [Dodatkowe materiały](#Dodatkowe-materiały) 
<!--te-->

## Jak zacząć projekt?

1. Upewnij się, że znasz podstawy wybranego języka programowania. Nie ma znaczenia, jaki język wybierzesz. Ważne jest, abyś dobrze opanował wszystkie podstawowe koncepcje i mechanizmy, takie jak zmienne, warunki, pętle, funkcje, tablice i napisy. Skorzystaj z dostępnych źródeł w internecie, aby znaleźć materiały dopasowane do swojego stylu nauki. Aby się uczyć, wykorzystaj różne ćwiczenia treningowe, które można znaleźć pod tym <a href="https://github.com/djeada/Nauka-Programowania">linkiem</a>.
2. Wybierz temat, który Cię interesuje. Szukaj inspiracji w swoim życiu codziennym. Na przykład możesz chcieć zautomatyzować wysyłanie e-maili lub stworzyć własną grę. Nie ma nic złego w czerpaniu pomysłów od innych programistów, ale nie kopiuj kodu. Strona <a href="https://github.com">GitHub</a> to dobre miejsce do poszukiwania inspiracji. Przyjrzyj się projektom stworzonym przez innych początkujących programistów i zastanów się, czy coś podobnego mogłoby Cię zainteresować.
3. Pamiętaj, że im bardziej oryginalny pomysł, tym lepiej. Świat widział już milion tetrisów, więc masz okazję wykazać się pomysłowością. Jeśli jednak nie przychodzi Ci do głowy żaden nowatorski pomysł, możesz skorzystać z istniejących pomysłów jako punktu wyjścia. Nie kopiuj jednak wszystkich rozwiązań; dodaj swoje pomysły i rozbuduj projekt, na którym się wzorujesz.
4. Oceń realność ukończenia wybranego projektu. Czy wiesz, jak zacząć? Jeśli nie, być może lepiej będzie zacząć od czegoś mniej ambitnego. Jeśli wiesz, jakie są kroki potrzebne do ukończenia projektu, spróbuj stworzyć listę i zapisz ją (na papierze lub w wersji elektronicznej).
5. Jeśli jeszcze tego nie zrobiłeś, pobierz odpowiednie środowisko programistyczne dla wybranego języka programowania. W większości przypadków dobrej jakości środowiskiem programistycznym jest <code>VS Code</code>. Pełną kontrolę nad wersjami języka oraz bibliotekami masz tylko wtedy, gdy pracujesz lokalnie, na swoim komputerze.
6. Na początku warto przygotować strukturę projektu, czyli określić, jakie pliki będą potrzebne do stworzenia aplikacji. Konkretny zestaw zależy od wybranego języka programowania. Na przykład, gdy tworzysz aplikację webową w czystym JavaScript, potrzebne będą minimum trzy pliki: index.html, index.js oraz index.css. Natomiast, jeśli wybrałeś Pythona, wystarczy, że utworzysz plik main.py oraz requirements.txt. Dla języków kompilowanych należy także napisać skrypt z instrukcjami budowania aplikacji. Taki skrypt określa, jakie pliki wynikowe powinny powstać w wyniku kompilacji i jak połączyć je z zewnętrznymi bibliotekami.
7. Bez względu na wybrany język, warto pamiętać o dokumentacji. Na początku warto stworzyć plik README.md, w którym opiszemy cel projektu, wymagane zależności oraz sposób obsługi interfejsu graficznego (można dodać krótkie demo w formacie GIF). W dalszej fazie pracy warto skorzystać z narzędzi generujących dokumentację, np. <a href ="https://www.sphinx-doc.org/en/master/">SPHINX</a> lub <a href="https://doxygen.nl/">Doxygen</a>.
8. Wszystkie przygotowane pliki warto umieścić w repozytorium Git.
9. Po wykonaniu powyższych kroków warto zrobić sobie chwilę przerwy i odpocząć.
10. Teraz nadszedł czas na implementację kluczowej części projektu - wcześniej przygotowanej listy kroków. Zanim jednak przystąpisz do pisania kodu, warto zastanowić się, czy niektóre punkty można podzielić na mniejsze lub czy coś nie zostało pominięte. W miarę rozwoju projektu prawdopodobnie będziesz modyfikował tę listę.
11. Implementuj funkcjonalności jedna po drugiej, a po każdej z nich przeprowadzaj ręczne testy swojego programu. Nie zakładaj, że wiesz, co robią Twoje linie kodu. Programowanie jest zmiennym procesem, a testy ręczne są podstawą. Ponadto warto rozważyć wprowadzenie testów automatycznych - większość języków programowania ma wiele bibliotek i frameworków, które pomagają w testowaniu programów. Pozwolą one szybko zweryfikować, czy nowe funkcjonalności nie powodują problemów z już istniejącymi. Automatyczne testy znacznie ułatwią również refaktoryzację.
12. Każda funkcjonalność powinna być zapisana jako osobny commit w repozytorium. Dzięki temu Twój postęp będzie archiwizowany, a Ty zawsze będziesz miał możliwość powrotu do ostatniej działającej wersji kodu. Umieszczenie repozytorium na zewnętrznym serwerze, takim jak GitHub, umożliwi Ci również dostęp do kopii zapasowych Twojej pracy.
13. Jeśli utkniesz, nie wahaj się skorzystać z pomocy Google. Umiejętność znalezienia odpowiedzi na techniczne pytania jest niezwykle ważna dla każdego programisty. Staraj się dokładnie opisać swój problem. Jeśli nie znajdziesz rozwiązania dla swojego problemu, spróbuj znaleźć rozwiązanie dla problemu bardziej ogólnego, a następnie przenieść je na Twój konkretny przypadek. W przypadku niszowych problemów, dla których nie ma rozwiązania na stronach typu <a href="https://stackoverflow.com/">Stack Overflow</a>, warto sięgnąć do dokumentacji i wykorzystać własne umiejętności.
14. Udostępnij swój projekt na platformie <a href="https://github.com">GitHub</a> lub, jeśli pracujesz nad aplikacją desktopową, przygotuj instalator. W przypadku aplikacji webowych warto rozważyć uruchomienie ich na zewnętrznym serwerze za pomocą darmowych narzędzi, takich jak <a href="https://devcenter.heroku.com/">Heroku</a>.
15. Daj mi znać o swoim projekcie!
    
## Tak zwane dobre praktyki

Dobre praktyki programowania to zestaw reguł, które mają na celu zwiększenie jakości kodu i ułatwienie jego dalszego rozwoju. Są one niezależne od konkretnego języka programowania.

### Obietnice dobrych praktyk

Stosowanie dobrych praktyk programowania przynosi wiele korzyści, w tym:

* Ulepszają czytelność kodu.
* Ułatwiają wprowadzanie zmian w kodzie.
* Minimalizują objętość kodu.
* Umożliwiają pracę w grupie.
* Ułatwiają testowanie programu.

### Organizacja projektu

Oto kilka dobrych praktyk organizacyjnych, które pomogą Ci utrzymać porządek i uporządkowanie w Twoim projekcie:

* Git to Twoje narzędzie do kontroli wersji - używaj go często i pracuj w małych krokach, zapisując swoją pracę za każdym razem z opisem naniesionych zmian. Jeśli pracujesz z zespołem, warto rozważyć rozgałęzienie projektu (ang. <a href="https://en.wikipedia.org/wiki/Branching_(version_control)">feature branches</a>). Inną dobrym podejściem jest regularne integrowanie drobnych zmian z główną gałęzią (ang. <a href="https://en.wikipedia.org/wiki/Continuous_integration">trunk-based development</a>).
* Czytaj kod, który już istnieje w projekcie i nie wahaj się wprowadzać zmian, jeśli uważasz, że coś da się zrobić lepiej lub prościej. Pamiętaj, że testy jednostkowe pomogą Ci uniknąć wprowadzenia destrukcyjnych zmian.
* Twórz dokumentację na bieżąco, dodając komentarze do tworzonych funkcji i klas. Kiedy napotkasz trudności, zapisz je i umieść krótkie streszczenie w pliku *README.md*.
* Dbaj o schludność i czytelność kodu. Użyj narzędzi automatycznej formatowania kodu, wybierz odpowiedni styl i trzymaj się go. Dziel kod między pliki, a pliki między foldery/moduły/paczki, a ich nazwy powinny być proste i opisowe.
* Rozdzielaj zadania między małe funkcje i klasy (jeśli programujesz w języku obiektowym).
* Nie kopiuj kodu z internetu bez zrozumienia. Wiele problemów, które napotkasz, zostało już rozwiązanych, ale fragmenty kodu, które znajdziesz, nie będą idealnie pasowały do Twojego projektu. Zamiast przeklejać kod, staraj się zrozumieć jego zasadę działania i napisać analogiczne rozwiązanie, które lepiej pasuje do Twojego projektu.
* Uważaj na martwy kod - nie twórz zmiennych, funkcji i klas, których nigdy nie użyjesz.
* Nie ignoruj ostrzeżeń kompilatora.


### Zmienne

* Nadawaj zmiennym nazwy, które mają sens. Nikt nie wie, co oznaczają zmienne o nazwach takich jak <code>a</code> lub <code>xob</code>. Nie przesadzaj jednak z długością nazw. Jeśli tworzysz zmienną <code>sumaZarobkowWszystkichPracownikowFirmy</code>, tylko po to, by użyć jej raz lub dwa razy, to może warto skrócić tę nazwę.
* Trzymaj się jednej konwencji nazewnictwa. Unikaj mieszania <code>snake_case</code> z <code>camelCase</code>.
* Unikaj tworzenia zbędnych zmiennych. Jeśli jakaś informacja jest już przechowywana w istniejących zmiennych, nie twórz dla niej nowych.
* Nie zmieniaj nagle znaczenia zmiennych. Jeśli zmienna <code>suma</code> przechowywała jakąś sumę, nie wykorzystuj jej do przechowywania średniej.
* Unikaj zmiennych globalnych. Szybko tracisz nad nimi kontrolę i mogą prowadzić do nieprzewidywalnego zachowania programu.
* Deklaruj zmienne tak blisko miejsca ich użycia, jak to możliwe.

### Warunki

* Nadmiernie skomplikowane warunki są trudne do zrozumienia i utrzymania. Jeśli chcesz jednocześnie sprawdzić kilka rzeczy i łączysz je za pomocą operatorów logicznych, lepiej wydzielić to zadanie do osobnej funkcji.
* Unikaj zagnieżdżania warunków. Lepiej użyj klauzul ochronnych (ang. guard clauses), które pozwalają uniknąć zagnieżdżania i poprawiają czytelność kodu.
* Staraj się nie umieszczać wielu warunków jednego pod drugim. Lepiej rozdziel ich zadania na osobne funkcje, co pozwoli na łatwiejsze utrzymanie i testowanie kodu.
* Nie porównuj wyniku wywołania funkcji zwracającej typ logiczny (prawda lub fałsz) do wartości logicznych w warunkach. Zamiast tego bezpośrednio użyj wyniku wywołania funkcji w warunku.
* Stosuj nawiasy wokół warunków, aby uniknąć niejasności i zapewnić porządek w kodzie.
* Unikaj negacji złożonych warunków, ponieważ mogą one być trudne do zrozumienia i prowadzić do błędów. Lepiej sformułować warunek w pozytywny sposób, gdyż zazwyczaj jest on łatwiejszy do zrozumienia.

### Funkcje

* Nadawaj funkcjom nazwy, które opisują czynność, którą wykonują.
* Każda funkcja powinna mieć jasno określony cel. Aby upewnić się, że cel został zrealizowany, dodaj testy jednostkowe.
* Nie stosuj nazw, które mogą wprowadzić czytelnika w błąd. Jeśli twoja funkcja nazywa się <code>czyPierwsza()</code>, to używaj jej tylko do sprawdzania, czy podana liczba jest liczbą pierwszą.
* Zachowaj zasadę DRY (Don't Repeat Yourself). Jeśli widzisz, że w kodzie wielokrotnie powtarza się ta sama kombinacja komend, przenieś je do osobnej funkcji.
* Im krótsza funkcja, tym lepiej. Nie rozciągaj definicji funkcji na dziesiątki wierszy. Nie ma jednak jednej liczby, która byłaby absolutnym maksimum liczby wierszy w funkcji.
* Funkcje powinny ukrywać szczegóły swojej implementacji. Podobnie jak przy naciskaniu przycisku na pilocie telewizora, nie powinniśmy wiedzieć, jak działa elektronika, z którą wchodzimy w interakcję. Powinniśmy znać tylko oczekiwany efekt. Dobre funkcje działają w ten sposób. Złe funkcje natomiast, ze względu na niejasną nazwę lub skomplikowane argumenty, wymagają od nas zrozumienia implementacji, aby poprawnie ich użyć.
* Podczas wywoływania funkcji przekazuj referencje do obiektów tam, gdzie niepotrzebna jest kopia. W wielu językach programowania, takich jak Python, jest to zachowanie domyślne, ale warto mieć świadomość tego faktu.
* Dbaj o spójność przekazywanych danych między funkcjami.
* Pamiętaj o przypadkach granicznych. Spełnienie celu stworzenia funkcji to tylko połowa sukcesu. Musisz również zastanowić się, czy klient funkcji może przekazać dane, dla których funkcja nie zwróci poprawnego wyniku lub które spowodują błąd, który zatrzyma działanie całego programu. Nawet jeśli prawdopodobieństwo wystąpienia takiego przypadku jest niskie, musisz zadbać o to, aby funkcja była w stanie sobie z nim poradzić.
* Unikaj argumentów typu logicznego. Lepiej podzielić funkcję na dwie mniejsze: jedną dla przypadku, gdy wartość argumentu wynosi prawda, a drugą dla przypadku, gdy wartość argumentu wynosi fałsz.
* Nie ma ścisłego limitu na ilość argumentów funkcji, ale im więcej argumentów, tym łatwiej się w nich pogubić. Dla funkcji, które wymagają wielu argumentów, najczęściej wiele z nich ma zbliżony cel, co umożliwia pogrupowanie ich w klasy/struktury.  
* Funkcje, które nie powodują efektów ubocznych, są uważane za najlepsze. Oznacza to, że wywołanie funkcji nie wpływa na stan programu poza samą funkcją.

### Klasy

* Nazywaj klasy rzeczownikami.
* Ukrywaj złożoność w klasach. Interfejs, zależności i efekty uboczne powinny być niewielkie w stosunku do złożoności ukrytej przez klasę. Unikaj tworzenia klas, których użycie jest trudniejsze niż bezpośrednia praca na danych, które przechowuje funkcja proceduralna.
* Klasa powinna ukrywać dane i udostępniać interfejs do operowania na tych danych. Nie mieszać filozofii programowania. Używaj klas do celów, dla których zostały stworzone.
* Grupuj dane w małe jednostki. Nie próbuj łączyć wszystkich potrzebnych danych w jednej klasie. W przeciwnym razie utrzymanie takiego kodu stanie się trudniejsze niż złamanie szyfru enigmy.
* Nie twórz klas, które nie są potrzebne, chyba że programujesz w języku, w którym wszystko musi być zamknięte w klasie. Jeśli nie potrzebujesz klasy, to nie twórz jej. Czasami zwykła funkcja jest wystarczająca.
* Unikaj funkcji, które zmieniają stan obiektu.
* Jeśli dwie funkcje w różnych klasach mają dokładnie to samo zadanie, to powinny mieć takie samo nazewnictwo.
* Unikaj tworzenia klas składających się z pól, które często są puste. Zamiast tego użyj wspólnego interfejsu dla kilku klas.
* Unikaj tworzenia kilku różnych klas, które mogą być jedną klasą, a ich obiekty różnią się jedynie wartością pola. Lepiej mieć jedną klasę z polami takimi jak kolor, niż wiele klas, takich jak <code>CzerwonyPrzycisk</code>, <code>ZielonyPrzycisk</code> i <code>NiebieskiPrzycisk</code>.
* Klasy są po to, by pod jedną nazwą zamknąć dane oraz transformacje dostępne dla tych danych. Dla jednorazowej modyfikacji użyj funkcji.

### Komentarze 

* Koniecznie umieszczaj w kodzie komentarze służące do generowania dokumentacji publicznego API (docstrings).
* Unikaj dodawania niepotrzebnych komentarzy. Zbędne komentarze to takie, które wyjaśniają znaczenie składni języka programowania, zamiast wyjaśniać zamierzenia autora. Komentarze służą do pokazania innym programistom, dlaczego dana decyzja została podjęta. Kod jest po to, by pokazać, jak została zaimplementowana. Na przykład, jeśli w kodzie zwiększamy zmienną indeks o 2, aby uwzględnić przesunięcie wprowadzone przez nagłówek, to należy to objaśnić w komentarzu, zamiast pisać, że zmienna zostaje powiększona.
* Unikaj komentarzy dezinformujących. Programista może celowo chcieć opisać jedynie część prawdy, ale niepoprawny komentarz może być również wynikiem braku aktualizacji przy wprowadzaniu zmian w kodzie.
* Akceptowalne są komentarze TODO z listą zadań do wykonania.
* Użyj komentarzy w celu wyjaśnienia trudnych do zrozumienia części kodu lub w celu objaśnienia algorytmów. Jednakże, stwórz kod, który jest łatwy do zrozumienia bez konieczności korzystania z komentarzy.
* Komentarze powinny być krótkie i zwięzłe, nie powinny zajmować więcej niż kilka linii.
* Staraj się unikać komentarzy w kodzie, który jest przeznaczony do zautomatyzowanego testowania. Testy powinny być łatwe do zrozumienia i czytania bez konieczności korzystania z komentarzy.

### Obsługa błędów

* Strategia obsługi błędów powinna być dostosowana do konkretnego języka programowania. Większość nowoczesnych języków posiada mechanizm wyjątków, który nie jest dostępny w starszych językach, takich jak C. Jednakże, sposób korzystania z wyjątków i sytuacje, w których powinny być one użyte, mogą się różnić między językami.
* Ogólnie rzecz biorąc, sytuacja wyjątkowa występuje, gdy funkcja nie jest w stanie w sposób sensowny poradzić sobie z pewnym problemem. Na przykład, jeśli piszemy funkcję zwracającą wartość elementu listy dla indeksu podanego jako parametr, nie możemy zwrócić prawidłowej wartości dla indeksów mniejszych niż zero lub większych niż długość listy. W takim przypadku powinniśmy zgłosić wyjątek <code>IndexOutOfBound</code>.
* Używanie wyjątków jest ogólnie preferowane nad zwracaniem kodów błędu lub wartości NULL/None w przypadku niepowodzenia funkcji.
* Należy unikać nadużywania wyjątków.
* Przy zgłaszaniu wyjątków należy poinformować użytkownika o wystąpieniu błędu, a jeśli to możliwe, wyjaśnić przyczynę błędu i wskazać dokładne miejsce w kodzie, gdzie wystąpił problematyczny wywołanie funkcji.
* Jeśli wywołujemy funkcję, która może zgłosić wyjątek, należy ją opatrzyć mechanizmem obsługi danego wyjątku.
* Należy unikać przekazywania wartości NULL/None do funkcji, ponieważ często prowadzi to do pojawiania się wyjątków, takich jak <code>NullPointerException</code>.

### Struktury danych

* Wybieraj odpowiednią strukturę danych do rozwiązania danego problemu. Dostępne struktury danych różnią się pod kątem szybkości dostępu, wstawiania, usuwania elementów, czy przeszukiwania, a także sposobu przechowywania danych (np. w postaci listy, tablicy, słownika).
* W wielu przypadkach prosta lista lub tablica jest wystarczająca i nie ma potrzeby używania bardziej skomplikowanych struktur danych.
* Jeśli potrzebujesz przechowywać elementy z dostępem poprzez klucz (np. indeksy nie są kolejnymi liczbami naturalnymi), użyj słownika (mapy).
* Zbiór (set) pozwala na przechowywanie tylko unikalnych elementów, jednak traci się wtedy informację o kolejności elementów.
* * W przypadku dużych zbiorów danych, w których trzeba szybko wyszukiwać i dodawać elementy, należy rozważyć użycie drzew binarnych lub tablic mieszających.
* Kolejki są dobrym wyborem, jeśli chcemy zachować porządek dodawania elementów i usuwania ich w kolejności dodania.
* Stosy to struktury danych, które stosujemy wtedy, gdy chcemy, aby ostatnio dodany element był pierwszy, który zostanie usunięty (Last-In-First-Out).
* Krotki (tuple) są używane, gdy chcemy przechowywać wiele wartości, ale nie chcemy, aby były one modyfikowalne. Są one często wykorzystywane do zwracania wartości z funkcji.
* Listy powiązane są przydatne w przypadku, gdy potrzebujemy przechowywać wiele elementów i chcemy mieć szybki dostęp do nich, ale nie wiemy, jak wiele elementów będziemy potrzebować na początku.
* Grafy są używane do reprezentowania zależności między elementami. Mogą być skierowane lub nieskierowane i składają się z wierzchołków oraz krawędzi.
* Kolejki priorytetowe są szczególnym rodzajem kolejek, w których elementy są ułożone w kolejności malejącej lub rosnącej wartości priorytetu.

### Testy

* Napisz testy jednostkowe dla każdej funkcji w celu sprawdzenia, czy wykonuje ona swoje zadanie zgodnie z oczekiwaniami.
* Unikaj duplikacji testów. Każde zadanie powinno być sprawdzone tylko raz.
* Testy są równie ważne jak kod produkcyjny.
* Dbaj o czytelność testów i ich odpowiednią organizację.
* Testy jednostkowe powinny mieć krótki czas wykonania.
* Upewnij się, że testy są niezależne od siebie i niezależne od środowiska, w którym są uruchamiane.
* Nie polegaj wyłącznie na testach jednostkowych. Warto zastanowić się także nad testami integracyjnymi i testami akceptacyjnymi.
* Unikaj używania funkcji <code>assert</code> do sprawdzania warunków w czasie wykonywania programu.

## Lista projektów

Celem tej sekcji jest zachęcenie czytelników do wypróbowania swoich sił w programowaniu i rozwijania swoich umiejętności poprzez realizację konkretnych projektów.

W sekcji znajduje się kilka różnych projektów, różniących się swoją tematyką i złożonością. Każdy z projektów posiada krótki opis, który wyjaśnia, czym jest projekt i jakie wymagania musi spełniać. Opisy te mogą zawierać również przykłady zastosowań projektu oraz podstawowe informacje na temat sposobu działania.

Projekty nie zostały uporządkowane względem żadnego kryterium.

### Szubienica

Jest to gra polegająca na odgadywaniu słów. Komputer losuje słowo z wcześniej przygotowanej listy słów. Na ekranie wyświetlane jest n kresek, gdzie n to liczba liter w wylosowanym słowie. Gracz ma x szans na odgadnięcie całego słowa. W każdej turze gracz podaje jedną literę. Jeśli odgadnie jedną z liter, które występują w wylosowanym słowie, to poziome kreseczki odpowiadające danej literze zostają zamienione na tę literę. W przeciwnym razie, liczba x jest zmniejszana o 1. Gracz wygrywa, jeśli odgadnie wszystkie litery przed wykorzystaniem się szans.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![hangman](https://user-images.githubusercontent.com/37275728/194822831-d1b117cb-ae01-4939-bac1-85ac4e58769a.gif)< | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/hangman)**
 
</div>

### Szyfr Cezara

Głównym elementem interfejsu graficznego jest pole tekstowe, w którym użytkownik może wpisać lub wkleić tekst. Następnie może wybrać jedną z dwóch opcji: szyfrowanie lub deszyfrowanie. W przypadku obu opcji użytkownik powinien również podać wartość klucza. Po wypełnieniu wszystkich pól wyświetlany jest przetworzony tekst. Algorytm szyfru Cezara działa w tle i jest używany do szyfrowania i deszyfrowania tekstu.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![caesar cipher](https://user-images.githubusercontent.com/37275728/194821911-e403023e-c5e5-4b19-b8bb-5cfedae8f164.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" /> | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/caesar_cipher)**
 
</div>

### Kalkulator

Kalkulator to aplikacja umożliwiająca wykonywanie podstawowych operacji matematycznych, takich jak dodawanie, odejmowanie, mnożenie i dzielenie. Interfejs graficzny kalkulatora jest intuicyjny i łatwy w obsłudze. Użytkownik może wprowadzać liczby za pomocą przycisków numerycznych, a następnie wybierać operację matematyczną, którą chce wykonać, korzystając z przycisków funkcyjnych.

Warto zauważyć, że kalkulator obsługuje nie tylko liczby całkowite, ale także liczby zmiennoprzecinkowe. Dodatkowo, w przypadku dzielenia przez zero lub przekroczenia zakresu liczb, kalkulator wyświetla odpowiednie komunikaty ostrzegawcze.

#### Linki

<div align="center">
 
Screenshot | Technologie | Link 
---|---|---
<img width="2000"/>![calculator](https://user-images.githubusercontent.com/37275728/194822287-7b84368a-2df0-4f4f-87a0-31951b91a253.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/calculator)**
 
</div>

### Lista zadań

Ta aplikacja umożliwia tworzenie, wyświetlanie, edycję i usuwanie zadań z listy. Zadania mogą być uporządkowane według priorytetów, dat wykonania lub kategorii, a użytkownik może filtrować zadania według tych kryteriów. Aplikacja umożliwia również dodawanie załączników do zadań, takich jak pliki lub zdjęcia. Zadania i ich szczegóły są przechowywane w bazie danych, co umożliwia łatwe zarządzanie nimi i wyszukiwanie ich według różnych kryteriów. Użytkownik może tworzyć nowe zadania, edytować istniejące i usuwać niepotrzebne. Aplikacja może również wysyłać powiadomienia o przypominającym terminie wykonania zadania.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Pogoda

Aplikacja pozwala na sprawdzenie aktualnej pogody dla wybranej lokalizacji. Użytkownik może wprowadzić nazwę miasta lub kod pocztowy, a następnie uzyskać informacje o temperaturze, wilgotności powietrza, prędkości wiatru, ciśnieniu atmosferycznym, a także godzinach wschodu i zachodu słońca dla danej lokacji. Dodatkowo, w aplikacji można sprawdzić prognozę pogody na kolejne dni oraz wyświetlić mapę z widocznymi danymi pogodowymi dla danego regionu. Aplikacja korzysta z zewnętrznego API do pobierania danych pogodowych, a interfejs graficzny jest prosty i intuicyjny w obsłudze.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Stoper

Jest to prosta aplikacja pozwalająca na odmierzanie czasu. Interfejs graficzny zawiera przyciski umożliwiające uruchomienie, zatrzymanie i resetowanie stopera oraz pole tekstowe, gdzie wyświetlany jest aktualny czas z dokładnością do milisekund.

Dodatkowo, użytkownik może ustawić czas startowy, po którym stoper samoczynnie się zatrzyma. Istnieje również możliwość wyboru formatu wyświetlanego czasu (np. minuty, sekundy, milisekundy) oraz zapisu wyników do pliku tekstowego.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Statki

Gra polegająca na umieszczeniu floty na planszy i zatopieniu statków przeciwnika. Gracz ustawia swoje statki na planszy, a pozycje statków komputera są losowane. W każdej turze gracz wybiera jedno pole na planszy, które próbuje trafic. Komputer również wybiera losowe pola na planszy gracza, które próbuje trafic. Gra toczy się, aż jeden z graczy zatopi wszystkie statki przeciwnika. W przypadku trafienia statku, gracz kontynuuje swoją turę, w przeciwnym razie przeciwnik otrzymuje swoją kolej. Wygrywa gracz, który pierwszy zatopi wszystkie statki przeciwnika.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Kółko i krzyżyk

Gra w kółko i krzyżyk polega na umieszczaniu kółek lub krzyżyków na planszy. Dostępne są dwa tryby gry: dla dwóch graczy lub z komputerem. Gracze na zmianę umieszczają swoje znaki na planszy, a zwycięzcą jest ten, kto jako pierwszy ustawił trzy swoje symbole w linii poziomej, pionowej lub na ukos. Jeśli na planszy nie ma już wolnych pól i żaden z graczy nie udał się uzyskać wymaganej linii, mamy remis. W trybie gry z komputerem, gracz rywalizuje z algorytmem sztucznej inteligencji, który stara się wygrać lub zremisować.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Saper

Saper to gra, która polega na odkrywaniu pól na planszy. Plansza jest wypełniona minami, a gracz musi uważać, aby nie odkryć pola z miną. Na planszy wyświetlane są liczby, które wskazują, ile min sąsiaduje z danym polem. Gracz może również umieścić flagę na polach, na których podejrzewa, że znajduje się mina. Jeśli gracz oznaczył wszystkie pola z minami flagami i odkrył wszystkie bezpieczne pola, wygrywa grę.

#### Linki
 
<div align="center">

Screenshot | Technologie | Link 
---|---|---
<img width="2000"/>![minesweeper](https://user-images.githubusercontent.com/37275728/194823180-a96946b2-082e-4aac-85cd-e822b6cf58c4.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/minesweeper)**
 
</div>

### 2048

2048 to gra logiczna, w której celem jest uzyskanie kwadratu o numerze 2048 na planszy 4x4, przesuwając mniejsze kwadraty z numerami, które łączą się, tworząc nowe kwadraty z wyższymi numerami. Gracz zaczyna z dwoma kwadratami o numerze 2 lub 4 i po każdym ruchu pojawia się nowy kwadrat. Gracz przesuwa kwadraty w jednym z czterech kierunków (góra, dół, lewo, prawo), a kwadraty poruszają się tak daleko, jak to możliwe, aż napotkają przeszkodę lub krawędź planszy.

W grze można łączyć kwadraty z tym samym numerem, co prowadzi do powstania kwadratu o numerze równym sumie numerów kwadratów składowych. Gra kończy się, gdy plansza zostanie wypełniona i gracz nie może wykonać żadnego ruchu, lub gdy gracz uzyska kwadrat o numerze 2048. Gracz może zagrać ponownie, aby uzyskać wyższy wynik lub spróbować zdobyć większy kwadrat, np. 4096.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Apokalipsa Zombie

Celem gry jest przetrwanie jak największej liczby fal zombie. Z każdą kolejną falą pojawia się coraz więcej i silniejszych przeciwników, co stawia przed graczem coraz większe wyzwania. Aby przetrwać, gracz musi skutecznie eliminować zombie przy użyciu różnych broni, zbierać bonusy zwiększające jego zdolności i zdobywać punkty. Oprócz standardowych broni, gracz może również korzystać z alternatywnych opcji takich jak granaty, moździerze czy miny. W grze istnieje również możliwość zdobycia dodatkowego życia oraz zwiększenia prędkości gracza. Wraz z każdą falą zombie, gra staje się coraz trudniejsza, co wymaga od gracza szybkości i skuteczności w działaniu.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![zombie_apocalypse](https://user-images.githubusercontent.com/37275728/188334905-179b94fd-eec2-44b8-a64f-fecdd6c6ea01.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/zombie_apocalypse)**
 
</div>

### Piętnastka

Gra polega na ułożeniu na planszy o wymiarach 4x4 kwadratów z numerami od 1 do 15 oraz jednym pustym polem w kolejności rosnącej. Gracz może przesuwać kwadraty znajdujące się obok pustego pola, w celu uzyskania właściwego ułożenia. Celem gry jest ułożenie kwadratów w kolejności rosnącej w jak najmniejszej liczbie ruchów.

#### Linki
 
<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![fifteen_puzzle](https://user-images.githubusercontent.com/37275728/194822577-fbfa5228-3643-4f61-ad69-bc58cd80b97a.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/fifteen_puzzle)**
 
</div>

### Kości

Gra turowa dla co najmniej dwóch graczy, w której używane są sześciościenną kości. Wszystkie możliwe kombinacje kości, wraz z liczbą punktów, jakie można za nie zdobyć, znajdują się w tabeli gry, a każdy gracz może użyć danej kombinacji dokładnie raz. W każdej turze gracz rzuca pięcioma kośćmi, a następnie ma możliwość rzucenia ponownie maksymalnie dwóch kości. Jeśli kości utworzyły jedną z pożądanych kombinacji, informacja o tym zostaje zapisana w tabeli gry. W przeciwnym razie gracz musi wykreślić jedną z możliwych kombinacji ze swojej tabeli. Pod koniec gry sumowane są punkty z tabeli dla każdego z graczy. Wygrywa gracz z największą liczbą punktów.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![yahtzee](https://user-images.githubusercontent.com/37275728/194823845-3aea219e-10d3-4d09-bc36-0832e7e0a8f8.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/yahtzee)**
 
</div>

### Kurka wodna

W grze Kurka wodna, gracze mają za zadanie ustrzelić każdą kurkę, która przelatuje przez ekran, klikając na nią myszką, zanim ta doleci na przeciwną stronę ekranu. Poziom trudności wzrasta z każdą kolejną falą kurek, a zmieniające się tła i animacje dodają grywalności.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Wąż

W grze węża gracz kontroluje węża składającego się z n kwadratów, który porusza się po prostokątnej planszy. Na planszy w określonych odstępach czasowych pojawia się jedzenie, a zjedzenie go przez węża zwiększa punkty i powiększa go o jeden kwadrat. Gracz przegrywa, gdy wąż uderzy swoją głową w część swojego ciała.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Edytor tekstowy

Edytor tekstu umożliwia wprowadzenie, edycję oraz wklejanie tekstu z klawiatury. Oprócz tego, użytkownik ma możliwość zapisania i wczytania swojego tekstu. Edytor posiada także podstawowe opcje formatowania tekstu, takie jak zmiana rozmiaru i rodzaju czcionki, pogrubienie, kursywa oraz podkreślenie.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Obróbka grafiki

Program umożliwia importowanie i wyświetlanie plików graficznych w formatach PNG i JPG. Posiada kilka narzędzi do edycji obrazów, takich jak rotacja, przycinanie i rozciąganie. W pasku narzędzi dostępne są różne narzędzia, w tym narzędzia do zmiany koloru, gumki, zaznaczania i pędzla. Program umożliwia eksportowanie zmodyfikowanych obrazów.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### System kontroli wersji

Ten program umożliwia prostą kontrolę wersji projektu. Pozwala na zapisanie bieżącego stanu projektu oraz na wyświetlenie historii wszystkich poprzednich zapisanych stanów. Dzięki temu użytkownik może łatwo cofnąć zmiany i przywrócić wcześniejszą wersję projektu.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Serwer HTTP
Serwer HTTP nasłuchuje na żądania HTTP kierowane pod adres URL http://127.0.0.1:8000/ i zwraca odpowiedzi. Może obsługiwać żądania GET, POST, PUT, DELETE i inne. W przypadku żądania GET, serwer może zwracać odpowiednie pliki, np. strony HTML, obrazy lub pliki JSON. Serwer HTTP może również działać w połączeniu z bazą danych, umożliwiając odczyt i zapis danych przy użyciu odpowiedniego API.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Koszyk z zakupami

Aplikacja umożliwia dodawanie, usuwanie oraz zmianę ilości produktów w koszyku. Wprowadzenie kodów rabatowych oraz kontynuacja zakupów są także dostępne. Produkty wyświetlane są z obrazkiem oraz informacją o cenie w uporządkowany sposób.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Zamawianie jedzenia

Aplikacja umożliwia przeglądanie listy dostępnych dań wraz z ich zdjęciami, cenami, opisami i tagami. Po wyborze interesującego nas dania musimy podać adres dostawy oraz wybrać formę płatności spośród PayPal, karty kredytowej lub gotówki przy odbiorze. Dodatkowo, opcjonalnie, można zaimplementować funkcję śledzenia statusu dostawy za pośrednictwem aplikacji.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Komunikator internetowy

Komunikator internetowy to aplikacja umożliwiająca wysyłanie tekstu pomiędzy komputerami w tej samej sieci LAN lub między dowolnymi urządzeniami z dostępem do internetu w bardziej zaawansowanej wersji. Użytkownicy mogą tworzyć konta, przesyłać pliki oraz korzystać z opcji wideorozmowy.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

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
* https://itp.uni-frankfurt.de/~mwagner/teaching/C_WS17/projects/Minesweeper.pdf

Przykłady plików readme:

* https://github.com/alecortega/portfolio-template
* https://github.com/othneildrew/Best-README-Template

Przykłady pełnych implementacji programów:

* https://github.com/Python-World/python-mini-projects
* https://github.com/swharden/Csharp-Data-Visualization
* https://github.com/practical-tutorials/project-based-learning
* https://github.com/bradtraversy/50projects50days
