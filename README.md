# Proste Projekty
Zbiór porad i zasobów dla początkujących programistów, zawierający szczegółowe wytyczne oraz przykłady realizacji projektów.

## Jak efektywnie rozpocząć swój pierwszy projekt?

1. **Znajomość podstaw**: Zacznij od solidnego opanowania podstaw wybranego języka programowania. Bez względu na wybór języka, kluczowe jest zrozumienie fundamentalnych koncepcji takich jak zmienne, instrukcje warunkowe, pętle, funkcje, tablice i napisy. Poszukaj materiałów edukacyjnych dostosowanych do Twojego stylu uczenia się, korzystając z różnorodnych zasobów online, w tym praktycznych ćwiczeń, które można znaleźć [tutaj](https://github.com/djeada/Nauka-Programowania).

2. **Wybór tematu**: Zainspiruj się swoim codziennym życiem lub pomysłami innych programistów, ale pamiętaj o tworzeniu oryginalnego kodu. Strona [GitHub](https://github.com) może być świetnym źródłem inspiracji. Zastanów się nad projektem, który Cię interesuje, czy to automatyzacja procesów, tworzenie gier, czy coś zupełnie innego.

3. **Oryginalność i innowacja**: Dąż do unikalności projektu. Świat widział już wiele klasycznych aplikacji, więc jest to szansa na wykazanie się kreatywnością. Jeśli jednak brakuje Ci pomysłów, możesz modyfikować i rozwijać istniejące koncepcje, wprowadzając swoje innowacje.

4. **Realność projektu**: Ocen, czy posiadasz wystarczającą wiedzę i umiejętności do realizacji zamierzonego projektu. Jeśli jesteś na etapie początkowym, zacznij od mniejszych i prostszych zadań.

5. **Środowisko programistyczne**: Pobierz i zainstaluj odpowiednie środowisko programistyczne, jak np. [VS Code](https://code.visualstudio.com/), które jest wszechstronne i dobrze wspiera wiele języków programowania.

6. **Struktura projektu**: Przygotuj podstawową strukturę projektu, określając niezbędne pliki i foldery w zależności od wybranego języka programowania.

7. **Dokumentacja**: Rozpocznij od stworzenia pliku README.md, opisując cel i zasady działania projektu. Z czasem wykorzystaj narzędzia do generowania dokumentacji, takie jak [SPHINX](https://www.sphinx-doc.org/en/master/) lub [Doxygen](https://doxygen.nl/).

8. **Użycie Git**: Zarządzaj kodem za pomocą systemu kontroli wersji Git, tworząc repozytorium dla swojego projektu.

9. **Przerwa i refleksja**: Zrób sobie krótką przerwę, aby następnie spojrzeć na projekt świeżym okiem.

10. **Rozwój projektu**: Implementuj funkcjonalności zgodnie z wcześniej przygotowaną listą zadań, dokonując ewentualnych modyfikacji i rozbudowy tej listy w miarę postępów pracy.

11. **Testowanie**: Regularnie testuj swój kod, zarówno ręcznie, jak i przy użyciu automatycznych narzędzi testujących. Jest to kluczowe dla utrzymania jakości i stabilności projektu.

12. **Zapisywanie postępów**: Dokumentuj swoje postępy za pomocą commitów w Git, co pozwoli na łatwe śledzenie zmian i powrót do poprzednich wersji kodu.

13. **Szukanie pomocy**: W przypadku problemów nie wahaj się korzystać z zasobów internetowych, takich jak [Stack Overflow](https://stackoverflow.com/), oraz oficjalnych dokumentacji.

14. **Udostępnianie projektu**: Po zakończeniu, udostępnij swój projekt na platformie [GitHub](https://github.com) i rozważ publikację aplikacji webowej na serwerze zewnętrznym, korzystając z narzędzi takich jak [Heroku](https://devcenter.heroku.com/).

15. **Podziel się swoimi osiągnięciami**: Nie zapomnij opowiedzieć mi o swoim projekcie!

Przystąp do tworzenia, rozwijaj swoje umiejętności i ciesz się każdym krokiem procesu programowania!

## Tak zwane dobre praktyki programowania

Dobre praktyki programowania to zestaw wytycznych mających na celu poprawę jakości kodu i ułatwienie jego rozwoju, bez względu na język programowania. 

### Korzyści z dobrych praktyk

Stosowanie tych praktyk przynosi szereg korzyści, takich jak:

- **Poprawa czytelności kodu**: Ułatwia zrozumienie i utrzymanie kodu.
- **Łatwość wprowadzania zmian**: Uproszczenie procesu modyfikacji i aktualizacji kodu.
- **Zmniejszenie objętości kodu**: Skuteczniejsze i zwięzłe rozwiązania.
- **Praca zespołowa**: Ułatwienie współpracy i dzielenia się kodem.
- **Uproszczenie testowania**: Lepsza testowalność i szybsze wykrywanie błędów.

### Organizacja projektu

Aby efektywnie zarządzać projektem, warto stosować się do następujących zasad:

- **Kontrola wersji z Git**: Regularnie zapisuj postępy i opisuj zmiany. Przy pracy zespołowej rozważ wykorzystanie [feature branches](https://en.wikipedia.org/wiki/Branching_(version_control)) lub [trunk-based development](https://en.wikipedia.org/wiki/Continuous_integration).
- **Analiza i optymalizacja istniejącego kodu**: Czytaj i poprawiaj istniejący kod, korzystając z testów jednostkowych, aby uniknąć wprowadzania błędów.
- **Dokumentacja**: Dokumentuj kod w trakcie tworzenia, komentując funkcje i klasy. Zapisuj napotkane trudności i dołączaj streszczenia w README.md.
- **Czytelność i schludność kodu**: Używaj narzędzi do formatowania kodu i trzymaj się wybranego stylu. Rozdzielaj kod na pliki, foldery, moduły lub pakiety o prostej i opisowej nazwie.
- **Podział na funkcje i klasy**: W językach obiektowych dziel zadania na małe, jednoznaczne funkcje i klasy.
- **Rozumienie zewnętrznego kodu**: Nie kopiuj kodu z internetu bez zrozumienia jego działania. Zamiast tego, staraj się zrozumieć zasadę działania i dostosować rozwiązanie do swojego projektu.
- **Unikanie martwego kodu**: Nie twórz zbędnych zmiennych, funkcji czy klas.
- **Uwaga na ostrzeżenia kompilatora**: Nie ignoruj ostrzeżeń, mogą one wskazywać na potencjalne problemy w kodzie.

### Zmienne

- **Nazwy zmiennych**: Nazwy zmiennych powinny być znaczące i opisowe. Unikaj krótkich i niejasnych nazw, takich jak `a` lub `xob`, ale także nie przesadzaj z długością. Na przykład, zamiast `sumaZarobkowWszystkichPracownikowFirmy`, wybierz coś bardziej zwięzłego.
- **Konwencja nazewnictwa**: Bądź konsekwentny w stosowaniu konwencji nazewnictwa, np. `snake_case` lub `camelCase`, i unikaj ich mieszania.
- **Zbędne zmienne**: Unikaj tworzenia zmiennych, które nie są konieczne lub są nadmiarowe.
- **Stałość znaczenia**: Zachowaj spójność w znaczeniu zmiennej; np. jeśli `suma` służy do przechowywania sumy, nie używaj jej do innych celów.
- **Unikanie zmiennych globalnych**: Staraj się unikać zmiennych globalnych, które mogą prowadzić do problemów z zarządzaniem stanem i nieprzewidywalnym zachowaniem programu.
- **Deklaracja w odpowiednim miejscu**: Deklaruj zmienne jak najbliżej miejsca ich pierwszego użycia.

### Warunki

- **Prostota warunków**: Unikaj zbyt skomplikowanych warunków; rozważ ich podzielenie na prostsze funkcje.
- **Unikanie zagnieżdżenia**: Zamiast zagnieżdżać warunki, użyj klauzul ochronnych, które poprawiają czytelność i strukturę kodu.
- **Rozdział zadań**: Nie stosuj wielu warunków bezpośrednio jeden po drugim; lepiej je rozdzielić na oddzielne funkcje.
- **Bezpośrednie używanie wartości logicznych**: W przypadku funkcji zwracających wartości logiczne, używaj ich wyników bezpośrednio w warunkach.
- **Stosowanie nawiasów**: Używaj nawiasów wokół warunków dla uniknięcia niejasności.
- **Unikanie negacji skomplikowanych warunków**: Staraj się formułować warunki pozytywnie, ponieważ negacja złożonych warunków może być myląca.

### Funkcje

- **Nazewnictwo**: Funkcje powinny mieć nazwy wyraźnie wskazujące na ich działanie.
- **Jednoznaczny cel**: Każda funkcja powinna mieć wyraźnie określony i ograniczony zakres działania, wspierany testami jednostkowymi.
- **Unikanie mylących nazw**: Nazwy funkcji powinny odzwierciedlać ich rzeczywiste działanie.
- **Zasada DRY**: Unikaj powtarzania kodu; wydziel powtarzające się fragmenty do osobnych funkcji.
- **Krótkość funkcji**: Dążyj do tworzenia krótkich funkcji, unikając nadmiernie długich definicji.
- **Ukrywanie implementacji**: Funkcje powinny ukrywać szczegóły swojej implementacji, eksponując tylko oczekiwany efekt.
- **Optymalne przekazywanie argumentów**: Przekazuj referencje zamiast kopii, gdzie to możliwe i sensowne.
- **Spójność danych**: Zachowaj spójność danych przekazywanych pomiędzy funkcjami.
- **Uwzględnianie przypadków brzegowych**: Projektuj funkcje tak, aby radziły sobie z nieoczekiwanymi lub nietypowymi danymi.
- **Unikanie logicznych argumentów**: Rozważ podział funkcji na dwie osobne, jeśli używają one argumentów logicznych do sterowania zachowaniem.
- **Ograniczenie liczby argumentów**: Im mniej argumentów, tym lepsza czytelność i łatwiejsze zarządzanie funkcją.
- **Funkcje bez efektów ubocznych**: Dążyj do tworzenia funkcji, które nie wprowadzają zmian w stanie globalnym programu.

### Klasy

- **Nazewnictwo**: Klasy nazywaj rzeczownikami. Nazwa powinna odzwierciedlać funkcję lub rolę klasy.
- **Ukrywanie złożoności**: Klasy powinny ukrywać swoją wewnętrzną złożoność, udostępniając prosty interfejs do interakcji. Unikaj tworzenia klas, które są bardziej skomplikowane w użyciu niż bezpośrednia praca na danych.
- **Enkapsulacja**: Klasa powinna ukrywać swoje dane i oferować interfejs do ich manipulacji, nie mieszając różnych stylów programowania.
- **Grupowanie danych**: Dane w klasach powinny być uporządkowane i skupione na określonych funkcjach lub cechach, unikając tworzenia "klas-monolitów".
- **Niepotrzebne klasy**: Nie twórz klas, które nie są potrzebne. W niektórych przypadkach funkcje mogą wystarczyć.
- **Stan obiektu**: Unikaj funkcji w klasach, które zmieniają stan obiektu w nieprzewidywalny sposób.
- **Spójność nazewnictwa funkcji**: Utrzymuj konsekwentne nazewnictwo funkcji o podobnym zadaniu w różnych klasach.
- **Minimalizacja pustych pól**: Unikaj tworzenia klas z polami, które często są nieużywane. Rozważ użycie wspólnego interfejsu dla różnych klas.
- **Unikanie redundancji**: Zamiast tworzenia wielu podobnych klas z różnymi wartościami pola, stwórz jedną klasę z odpowiednimi polami.

### Komentarze

- **Komentarze do dokumentacji**: Umieszczaj komentarze docstrings w celu generowania dokumentacji API.
- **Unikanie zbędnych komentarzy**: Komentarze powinny wyjaśniać "dlaczego" zamiast "jak". Unikaj komentarzy tłumaczących składnię języka.
- **Ostrzeżenie przed dezinformacją**: Uważaj, aby komentarze nie wprowadzały w błąd; aktualizuj je wraz ze zmianami w kodzie.
- **Akceptacja komentarzy TODO**: Lista zadań do wykonania w formie komentarzy TODO jest akceptowalna.
- **Wyjaśnianie skomplikowanego kodu**: Używaj komentarzy do wyjaśnienia trudnych części kodu lub algorytmów.
- **Krótkie i zwięzłe komentarze**: Komentarze powinny być skoncentrowane i nie zajmować zbyt wiele miejsca.
- **Unikanie komentarzy w testach**: Testy powinny być na tyle jasne, że nie wymagają dodatkowych komentarzy.

### Obsługa błędów

- **Strategia zależna od języka**: Sposób obsługi błędów powinien być dostosowany do specyfiki danego języka programowania.
- **Wyjątki vs. kody błędów**: Preferuj użycie wyjątków zamiast zwracania kodów błędu lub wartości NULL/None.
- **Ograniczone stosowanie wyjątków**: Używaj wyjątków tylko w sytuacjach, gdy funkcja nie może sensownie zakończyć zadania.
- **Informowanie o błędach**: Wyjątki powinny dostarczać jasnych informacji o rodzaju i przyczynie błędu.
- **Obsługa wyjątków**: Gdy wywołujesz funkcję, która może zgłosić wyjątek, upewnij się, że jest on odpowiednio obsłużony.
- **Unikanie NULL/None**: Unikaj przekazywania wartości NULL/None do funkcji, aby zmniejszyć ryzyko wyjątków typu `NullPointerException`.

### Struktury danych

- **Dobór struktury danych**: Wybierz strukturę danych, która najlepiej odpowiada potrzebom danego problemu. Różne struktury danych mają różne właściwości i zastosowania, np. listy, tablice, słowniki, zbiory, drzewa binarne, tablice mieszające (hasztablice).
- **Proste struktury**: W wielu przypadkach wystarczające mogą być proste listy lub tablice.
- **Słowniki/Mapy**: Jeżeli potrzebujesz szybkiego dostępu do elementów za pomocą kluczy, użyj słownika lub mapy.
- **Zbiory**: Zbiory (sety) są używane do przechowywania unikalnych elementów, ale nie zachowują kolejności.
- **Drzewa binarne/Tablice mieszające**: W dużych zbiorach danych, gdzie kluczowe jest szybkie wyszukiwanie, rozważ użycie drzew binarnych lub tablic mieszających.
- **Kolejki**: Wykorzystuj kolejki, gdy chcesz zachować kolejność dodawania i usuwania elementów.
- **Stosy**: Stosy są idealne do zastosowań typu Last-In-First-Out.
- **Krotki**: Używaj krotek do przechowywania niemodyfikowalnych zbiorów wartości.
- **Listy powiązane**: Idealne do przechowywania dużych ilości elementów z możliwością łatwej modyfikacji.
- **Grafy**: Do reprezentowania złożonych zależności i relacji między elementami.
- **Kolejki priorytetowe**: Do zarządzania elementami z określonym priorytetem.

### Testy

- **Testy jednostkowe**: Pisząc testy jednostkowe, upewnij się, że sprawdzają one poszczególne funkcje pod kątem spełniania ich zamierzonych celów.
- **Unikaj duplikacji testów**: Każdy scenariusz testowy powinien być sprawdzony tylko raz.
- **Ważność testów**: Testy są równie ważne jak kod produkcyjny.
- **Czytelność i organizacja testów**: Utrzymuj testy w czytelnej i dobrze zorganizowanej formie.
- **Szybkość wykonania testów jednostkowych**: Testy jednostkowe powinny charakteryzować się krótkim czasem wykonania.
- **Niezależność testów**: Testy powinny być niezależne od siebie i od środowiska, w którym są uruchamiane.
- **Oprócz testów jednostkowych**: Rozważ wykorzystanie testów integracyjnych i akceptacyjnych.
- **Unikaj assert w kodzie produkcyjnym**: Funkcje `assert` nie powinny być używane do sprawdzania warunków w czasie działania programu.

## Lista projektów programistycznych

Celem tej sekcji jest zainspirowanie i motywowanie czytelników do aktywnego uczestnictwa w nauce programowania poprzez praktyczne realizowanie różnorodnych projektów. Projekty te różnią się tematyką i stopniem trudności, co pozwala każdemu znaleźć coś dla siebie.

### Szubienica

Projekt gry "Szubienica" polega na odgadywaniu słów. Komputer losowo wybiera słowo z przygotowanej listy, a następnie wyświetla na ekranie serie kresek (_), odpowiadających literom w wybranym słowie. Gracz ma ograniczoną liczbę prób (np. 10) na odgadnięcie całego słowa. W każdej turze gracz wybiera literę. Jeżeli litera jest w słowie, odpowiednie kreski są zastępowane przez tę literę. Jeśli litera nie występuje w słowie, liczba dostępnych prób gracza zmniejsza się o jedną. Gracz wygrywa, jeśli odgadnie wszystkie litery przed wyczerpaniem wszystkich szans.

Projekt ten pozwala na ćwiczenie podstawowych umiejętności programowania, takich jak operacje na łańcuchach znaków, kontrola przepływu programu oraz prosty algorytm losowania. Dodatkowo, można rozbudować grę o różne funkcjonalności, jak np. różne poziomy trudności, podpowiedzi czy graficzne reprezentacje postępów gracza.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![hangman](https://user-images.githubusercontent.com/37275728/194822831-d1b117cb-ae01-4939-bac1-85ac4e58769a.gif)< | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/hangman)**
 
</div>

### Szyfr Cezara

Projekt Szyfru Cezara to aplikacja umożliwiająca szyfrowanie i deszyfrowanie tekstu przy użyciu jednej z najstarszych znanych metod kryptografii. Interfejs graficzny zawiera pole tekstowe, w którym użytkownik wpisuje tekst do zaszyfrowania lub odszyfrowania. Po wpisaniu tekstu, użytkownik wybiera opcję szyfrowania lub deszyfrowania i podaje wartość klucza przesunięcia, która określa, o ile pozycji w alfabecie przesunąć każdą literę tekstu. Przetworzony tekst jest następnie wyświetlany w interfejsie. Szyfr Cezara jest prostym przykładem szyfru podstawieniowego i jest świetnym sposobem na zrozumienie podstaw kryptografii.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![caesar cipher](https://user-images.githubusercontent.com/37275728/194821911-e403023e-c5e5-4b19-b8bb-5cfedae8f164.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" /> | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/caesar_cipher)**
 
</div>

### Kalkulator

Kalkulator to podstawowy projekt, który pomaga w zrozumieniu obsługi interfejsu użytkownika i podstawowych operacji arytmetycznych. Aplikacja pozwala na wykonanie podstawowych operacji matematycznych, takich jak dodawanie, odejmowanie, mnożenie i dzielenie. Interfejs graficzny jest intuicyjny, z przyciskami numerycznymi do wprowadzania liczb oraz przyciskami funkcyjnymi do wyboru operacji. Kalkulator obsługuje zarówno liczby całkowite, jak i zmiennoprzecinkowe, a także wyświetla komunikaty ostrzegawcze w przypadku błędów, takich jak dzielenie przez zero. Ten projekt jest doskonałym sposobem na naukę podstaw obsługi zdarzeń i logiki programistycznej.

#### Linki

<div align="center">
 
Screenshot | Technologie | Link 
---|---|---
<img width="2000"/>![calculator](https://user-images.githubusercontent.com/37275728/194822287-7b84368a-2df0-4f4f-87a0-31951b91a253.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/calculator)**
 
</div>

### Lista zadań

Projekt listy zadań to aplikacja do zarządzania zadaniami, która umożliwia użytkownikom tworzenie, edycję, usuwanie oraz sortowanie zadań według różnych kryteriów. Można ustawić priorytety, daty wykonania oraz kategorie dla zadań. Aplikacja umożliwia także dodawanie załączników do zadań, co pozwala na lepsze zorganizowanie pracy. Wszystkie informacje o zadaniach są przechowywane w bazie danych, co ułatwia ich zarządzanie. Aplikacja może również wysyłać powiadomienia o zbliżających się terminach. Ten projekt jest doskonały do nauki pracy z bazami danych, interfejsami użytkownika i systemami powiadamiania.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Pogoda

Projekt aplikacji pogodowej umożliwia użytkownikom sprawdzanie aktualnej pogody oraz prognozy na najbliższe dni dla wybranej lokalizacji. Użytkownik może wprowadzić nazwę miasta lub kod pocztowy, by otrzymać dane takie jak temperatura, wilgotność powietrza, prędkość wiatru, ciśnienie atmosferyczne oraz godziny wschodu i zachodu słońca. Aplikacja zawiera także mapę z zaznaczonymi danymi pogodowymi dla danego regionu. Informacje pogodowe są pobierane z zewnętrznego API, a interfejs użytkownika jest prosty i intuicyjny. Projekt ten pozwala na naukę korzystania z zewnętrznych API, przetwarzania i wyświetlania danych, a także podstaw pracy z mapami.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Stoper

Stoper to prosta aplikacja do odmierzania czasu z dokładnością do milisekund. Posiada funkcjonalności takie jak start, stop, reset oraz możliwość ustawienia czasu, po którym stoper zatrzyma się automatycznie. Użytkownik może wybrać preferowany format wyświetlania czasu i zapisywać wyniki do pliku tekstowego. Interfejs graficzny składa się z przycisków do kontroli stopera oraz wyświetlacza czasu. Jest to doskonały projekt dla początkujących programistów, aby zrozumieć obsługę zdarzeń i pracę z czasem.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Statki

Gra w statki to klasyczna gra polegająca na umieszczaniu floty na planszy i próbie zatopienia statków przeciwnika. Gracz ustawia swoje statki na planszy, podczas gdy pozycje statków komputera są losowane. Gracze na zmianę wybierają pola, które próbują trafic. Gra oferuje możliwość grania przeciwko komputerowi lub drugiemu graczowi. Projekt ten jest doskonałym ćwiczeniem w programowaniu gier, algorytmach losowania oraz obsłudze zdarzeń użytkownika.
#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Kółko i krzyżyk

Kółko i krzyżyk to klasyczna gra, która może być realizowana w trybie dwóch graczy lub przeciwko komputerowi. Gracze na zmianę umieszczają swoje symbole (kółko lub krzyżyk) na planszy 3x3. Wygrywa ten, kto pierwszy ustawi trzy swoje symbole w linii. W trybie gry z komputerem, gracz rywalizuje z algorytmem AI. Projekt ten jest świetny do nauki podstaw logiki gier, algorytmów AI i obsługi interfejsu użytkownika.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Saper

Saper to klasyczna gra logiczna, w której zadaniem gracza jest odkrywanie pól na planszy bez detonowania min. Każde pole może zawierać minę lub liczbę wskazującą, ile min sąsiaduje z tym polem. Gracze używają tych liczb, aby bezpiecznie odkrywać kolejne pola. Gracz może także oznaczać pola, na których podejrzewa obecność miny, flagą. Gra kończy się wygraną, gdy wszystkie pola niezawierające min zostaną odkryte. Saper to doskonałe ćwiczenie w logice i strategicznym myśleniu, a także w obsłudze zdarzeń myszy i zarządzaniu stanem gry.

#### Linki
 
<div align="center">

Screenshot | Technologie | Link 
---|---|---
<img width="2000"/>![minesweeper](https://user-images.githubusercontent.com/37275728/194823180-a96946b2-082e-4aac-85cd-e822b6cf58c4.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/minesweeper)**
 
</div>

### 2048

2048 to popularna gra logiczna polegająca na łączeniu kafelków z takimi samymi numerami na planszy 4x4. Celem gry jest utworzenie kafelka z liczbą 2048. Gracz przesuwa kafelki w jednym z czterech kierunków, a gdy dwa kafelki o tym samym numerze zderzają się, łączą się w jeden o wartości będącej sumą połączonych kafelków. Po każdym ruchu pojawia się nowy kafelek o wartości 2 lub 4. Gra kończy się, gdy nie ma już możliwości wykonania ruchu lub gracz uzyska kafelek 2048. Projekt 2048 pozwala na naukę programowania animacji, obsługi zdarzeń klawiatury i algorytmów łączenia elementów.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Apokalipsa Zombie

Apokalipsa Zombie to gra akcji, w której celem gracza jest przetrwanie fal atakujących zombie. Z każdą falą, zombie stają się liczniejsze i silniejsze. Gracz musi wykorzystać różnorodne rodzaje broni i bonusy, aby przetrwać i zdobywać punkty. Gra zawiera różne poziomy trudności i umożliwia zdobywanie ulepszeń broni oraz bonusów, takich jak dodatkowe życie czy zwiększenie prędkości. Ten projekt może być interesującym wyzwaniem w zakresie tworzenia gier akcji, z naciskiem na algorytmy sterowania przeciwnikami, detekcję kolizji i zarządzanie zasobami gracza.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![zombie_apocalypse](https://user-images.githubusercontent.com/37275728/188334905-179b94fd-eec2-44b8-a64f-fecdd6c6ea01.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/zombie_apocalypse)**
 
</div>

### Piętnastka

Piętnastka to klasyczna gra logiczna, w której celem jest ułożenie 15 kwadratów z numerami w kolejności rosnącej na planszy 4x4, zostawiając jedno puste pole, które umożliwia przesuwanie kwadratów. Gracz przesuwa kwadraty wokół pustego pola, aby uzyskać prawidłowe ułożenie. Gra ta jest świetnym ćwiczeniem w algorytmach układania i myślenia przestrzennego. Dodatkowo, implementacja interfejsu użytkownika dla tej gry może pomóc w zrozumieniu obsługi zdarzeń dotykowych lub myszy oraz animacji.

#### Linki
 
<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![fifteen_puzzle](https://user-images.githubusercontent.com/37275728/194822577-fbfa5228-3643-4f61-ad69-bc58cd80b97a.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/fifteen_puzzle)**
 
</div>

### Kości

Kości to gra towarzyska, w której gracze rzucają zestawem pięciu kości, starając się uzyskać różne kombinacje przedstawione w tabeli punktacji. Gracz może rzucić kośćmi do trzech razy w swojej turze, aby uzyskać jedną z kombinacji, a następnie zapisuje wynik w swojej tabeli punktów. Kombinacje mogą być użyte tylko raz przez każdego gracza, a na koniec gry zwycięża osoba z najwyższą sumą punktów. Projekt ten jest doskonały do nauki obsługi losowości, zdarzeń i logicznego myślenia, a także oferuje możliwość tworzenia interfejsu użytkownika dla gier planszowych.

#### Linki

<div align="center">

Screenshot | Technologie | Link 
---|---|---
 <img width="2000"/>![yahtzee](https://user-images.githubusercontent.com/37275728/194823845-3aea219e-10d3-4d09-bc36-0832e7e0a8f8.gif) | <img src="https://img.icons8.com/color/344/python.png" height="50" />  | **[Link](https://github.com/djeada/Proste-Projekty/tree/main/projekty/python/yahtzee)**
 
</div>

### Kurka wodna

Kurka wodna to dynamiczna gra, w której gracze muszą "ustrzelić" kurki pojawiające się i przelatujące przez ekran, klikając na nie myszką, zanim przekroczą przeciwną stronę ekranu. Z każdą falą, pojawia się więcej kurek, a gra staje się trudniejsza. Projekt ten oferuje wspaniałą okazję do nauki programowania gier zręcznościowych, w tym obsługi zdarzeń myszy, animacji, zarządzania poziomami trudności i implementacji dynamicznych tła oraz elementów graficznych.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Wąż

W grze Wąż gracz steruje wężem poruszającym się po prostokątnej planszy. Cel gry polega na zjadaniu pojawiających się na planszy elementów, co powoduje wydłużanie węża i zdobywanie punktów. Gra staje się trudniejsza w miarę wzrostu węża, ponieważ gracz musi unikać uderzenia głową węża w jego własne ciało. Gra ta jest klasycznym przykładem prostych gier zręcznościowych i świetnie nadaje się do nauki podstaw programowania gier, w tym obsługi ruchu, kolizji oraz zarządzania stanem gry.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Edytor tekstowy

Projekt edytora tekstu oferuje możliwość tworzenia, edytowania i zapisywania tekstów. Użytkownik może również formatować tekst, zmieniając czcionkę, styl i rozmiar, a także dodawać pogrubienie, kursywę i podkreślenie. Jest to doskonałe ćwiczenie w obsłudze zdarzeń klawiatury, interfejsu użytkownika oraz podstawowych technik formatowania tekstu. Może również obejmować funkcjonalności takie jak wybór kolorów tekstu, listy punktowane czy wstawianie obrazów.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Obróbka grafiki

Program do obróbki grafiki umożliwia importowanie, wyświetlanie oraz edycję obrazów w formatach PNG i JPG. Użytkownik może korzystać z różnych narzędzi, takich jak rotacja, przycinanie, rozciąganie, zmiana koloru, gumka, zaznaczanie i pędzel. Projekt ten jest doskonałą okazją do nauki obsługi plików graficznych, implementacji narzędzi edycyjnych oraz podstaw grafiki komputerowej. Można również rozszerzyć projekt o bardziej zaawansowane funkcje, takie jak warstwy, filtry czy efekty specjalne.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### System kontroli wersji

Projekt systemu kontroli wersji oferuje podstawową funkcjonalność zarządzania wersjami projektu. Pozwala użytkownikowi na zapisywanie bieżącego stanu projektu i przeglądanie historii zapisów. Dzięki temu można łatwo przywracać wcześniejsze wersje projektu lub cofać wprowadzone zmiany. Jest to doskonałe ćwiczenie w zakresie zarządzania plikami, obsługi systemów plików oraz podstaw algorytmów związanych z kontrolą wersji.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Serwer HTTP

Projekt serwera HTTP polega na stworzeniu serwera, który nasłuchuje na żądania HTTP wysyłane do adresu URL, np. http://127.0.0.1:8000/, i zwraca odpowiedzi. Serwer może obsługiwać różne typy żądań, takie jak GET, POST, PUT i DELETE. W przypadku żądania GET, serwer może zwracać strony HTML, obrazy, pliki JSON lub inne typy plików. Serwer może być także połączony z bazą danych, umożliwiając tworzenie dynamicznych aplikacji internetowych z funkcjami odczytu i zapisu danych. Projekt ten jest doskonałym wprowadzeniem do programowania serwerowego, sieci i protokołów HTTP.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Koszyk z zakupami

Projekt koszyka z zakupami to aplikacja e-commerce, która umożliwia użytkownikowi dodawanie produktów do koszyka, usuwanie ich oraz zmianę ich ilości. Aplikacja umożliwia również stosowanie kodów rabatowych i prowadzenie przez proces zakupowy. Produkty są prezentowane w atrakcyjny sposób, z obrazkami i informacjami o cenach. Projekt ten pozwala na naukę tworzenia interfejsów użytkownika, zarządzania stanem aplikacji oraz integracji z systemami płatności i bazami danych.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Zamawianie jedzenia

Aplikacja do zamawiania jedzenia pozwala użytkownikom przeglądać menu z dostępnymi daniami, wraz z ich zdjęciami, cenami, opisami i kategoriami. Po wybraniu posiłku, użytkownik podaje adres dostawy i wybiera formę płatności - PayPal, karta kredytowa lub gotówka przy odbiorze. Aplikacja może również zawierać funkcję śledzenia statusu zamówienia w czasie rzeczywistym. Jest to kompleksowy projekt, który łączy w sobie zarządzanie bazą danych, interfejs użytkownika, obsługę płatności oraz integrację z zewnętrznymi usługami dostarczania.

#### Linki

Screenshot | Technologie | Link 
---|---|---
| - | - | - |

### Komunikator internetowy

Komunikator internetowy to aplikacja umożliwiająca komunikację tekstową, przesyłanie plików i wideorozmowy między użytkownikami. W prostszej wersji, użytkownicy mogą komunikować się w sieci LAN, a w bardziej zaawansowanej wersji - z dowolnym miejscem na świecie przez internet. Funkcje takie jak tworzenie kont, zarządzanie listą kontaktów i szyfrowanie komunikacji mogą zostać dodane, by zwiększyć funkcjonalność i bezpieczeństwo. Projekt ten oferuje praktyczne doświadczenie w pracy z sieciami, protokołami komunikacyjnymi oraz zabezpieczeniami danych.

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
