Eka osa:

Ohjelman tietorakenteet.

Struct datan säilyttämistä varten, unordered_set vasalleja varten
kaupunkien tiedot talletetaan structiin, koska sieltä niiden hakeminen on nopeaa
vasallit ovat structin sisällä unordered_setissä, usettiä käytetään, sillä sinne lisääminen
sieltä poistaminen ja sieltä hakeminen ovat kaikki keskimääräisesti vakioiaikaisia.
Struct sisältää osoittimet vasalleihin ja isäntään. tämä on sitä varten, että näin muodostuu
puu, joka mahdollistaa nopeamman selvityksen isännistä ja vasalleista. Tällä tavalla pääsee
myös vakioaikaisesti isännän ja tietyn vasallin structiin käsiksi. Viikolla 7 annettu artikkeli
osoittimien säilymisestä kertoo, että umap rehash ei riko viitteitä ja osoittimia.

En ole tehnyt muita tietorakenteita, sillä käytetyt tietorakenteet toimivat tehokkaasti tiedon hakemiseen
ja uusien rakenteiden luominen ja ylläpitäminen spesifejä tarkoituksia varten vie resursseja.

Kaupungit ja niiden data (id,nimi,vero,koordinaatit,isäntä,vasallit)
talletetaan unordered_map tietorakenteeseen, jossa avaimena toimii id ja
avaimen takana olevana arvona struct, joka sisältää kaupunkien datan. Näin tehdään, sillä
kaupunkia haetaan todella usein id:n perusteella, joka on keskimäärin vakioiaikaista umapista.

Ohjelman apumetodit ovat toteutettu joko sitä varten, että samanlaisia operaatioita tehdään
monessa eri paikassa ja tämän takia on kätevä vain kutsua apumetodia, tai jotta saisi tehtyä
rekursion järkevämmin hyödyntäen joko viitteenä annettua listaa (add_masters) tai osoittimia
(set_furthest_vassal).

Alkupaan metodit (jotka tulevat ennen towns_alphabetically) ovat yksiselitteisiä.
Towns_alphabetically ja get_distance_vector (etäisyysmetodien apumetodi) siirtävät
id - nimi/etäisyys pareja umapista nimi/etäisyys - id pareiksi multimappiin. Tällä tavalla
id:t saadaan haettua nimi/etäisyys -järjestyksessä ilman erillistä sorttausta, jolloin vaaditaan
vain parin tietorakenteen iteroimista ja inserttausta.

add_vassalship ja get_town_vassals ovat triviaaleja.

taxer_path kutsuu apumetodia add_masters lisäämään kaupunkeja vektoriin, joka palautetaan.
add_masters lisää saamansa kaupungin ID:n vektoriin ja kutsuu itseään rekursiivisesti antaen
ensimmäisen kaupungin isännän kunnes isäntiä ei enää ole, tai ollaan päästy haluttuun isäntään asti
Rekursiivinen toteutus tälle on tehokas ja toimiva.

remove_town on aika triviaali. Asetetaan tietyssä paikassa olevan structin arvoja ja lisätään/
poistetaan arvoja unordered_setistä.

longest_vassal_path kutsuu useampaa apumetodia selvitääkseen usealla lineaarisella operaatiolla
jotka eivät ole sisäkkäin (muuttuen silloin N^2). set_furthest_vassal käy läpi vasalli/isäntä
-suhteiden luomaa "puuta", syvyys ja syvin löydetty syvyys talletetaan metodin ulkopuolelle, jotta
säästyttäisiin turhilta parametreiltä. Jokaisen lehden läpikäynti rekursiivisesti ja alimman
lehden tallentaminen, jotta voisi kutsua aiemmin käytettyä add_masters käyttäen alinta lehtä
on tehokkain tapa minkä keksin ja on kokonaisuudessaan lineaarinen.

total_net_tax hakee kokonaisveron kutsuen rekursiivista apumetodia. Tehokkain tapa minkä
keksin oli käydä jokainen vasalli ja vasallin vasalli etc. rekursiivisesti läpi.

Toka osa:

Ohjelman tietorakenteet.

Oma umap graafin muodostusta varten. Ei kannata käsitellä jokaista kaupunkia tieoperaatioissa, kun voi käsitellä vain niitä,
joilla on teitä. Umap sisältää node-structeja, jotka taas sisältävät kaupungin ID:n muuttujia graafialgoritmeja varten, sekä umapin
viereisiin kaupunkeihin (on tie) sekä etäisyyden. Umapit ja structit ovat tehokas (Käytännössä vakioaikainen) tapa tallettaa
ja hakea tietoa tietyistä kaupungeista, sekä liikkua niiden naapureihin. Datastrucures.hh sisältää apumuuttujia, joiden avulla voidaan
pitää kirjaa mihin sykli loppui, mikä oli kaupunki johon olisi menty kaksi kertaa ja onko haluttu kaupunki löytynyt.

Apumetodeja on toteutettu selkeyttämään ohjelmaa, mahdollistamaan rekursiota tai helppoa poistumista silmukoista returnin avulla ja
usein käytettyjä operaatiota varten.

En tehnyt enempää tietorakenteita, sillä graafin sai nätisti talletettua umap struct rakenteeseen.

Clear_roads on triviaali.

All_roads: Kaupunkien ID:t yhdistetään tavalla, jonka pitäisi tehdä jokaisesta kaupunkiparista uniikin ja lisätty
umappiin, sillä umap on tehokas rakenne tähän ja tällä tavalla on helppo hankkiutua eroon duplikaattiteistä
(sama tie löytyy kummankin kaupungin tielistasta).

Add_road on aika triviaali, tarkastetaan onko olemassa ja lisätään.

Get_roads_from on triviaali.

Any_route tehdään tarvittavat tarkastukset ja alustukset, jonka jälkeen kutsutaan metodia, joka etsii reitin DFS menetelmällä
ja jättää osoittimet edellisiin. Sen jälkeen käydään nämä läpi ja talletetaan ID:t. Reitti on järkevää etsiä ensin ja sitten
Käydä reitti läpi ja tallettaa jokainen reitillä oleva kaupunki. Apumetodi toteutettu rekursiivisesti, sillä se tuntui
kätevältä ja suhtkoht tehokkaalta tavalta tehdä se.

Remove_road on aika triviaali, tehdään tarkistuksia ja poistetaan joko tie tai kaupunki kummankin kaupungin tapauksessa.

Least_towns_route: etsitään oikea reitti BFS algoritmia kutsumalla ja käydään reitti sitten läpi vähän kuten Any_route.

Road_cycle_route: logiikka hyvin samanlainen kuin any_route.

shortest_route: Taas samanlaista logiikka, mutta tällä kertaa kutsutaan dijkstran algoritmia.





    