Ohjelman tietorakenteet.

Struct datan säilyttämistä varten, unordered_set vasalleja varten
kaupunkien tiedot talletetaan structiin, koska sieltä niiden hakeminen on nopeaa
vasallit ovat structin sisällä unordered_setissä, usettiä käytetään, sillä sinne lisääminen
sieltä poistaminen ja sieltä hakeminen ovat kaikki keskimääräisesti vakioiaikaisia.
Struct sisältää osoittimet vasalleihin ja isäntään. tämä on sitä varten, että näin muodostuu
puu, joka mahdollistaa nopeamman selvityksen isännistä ja vasalleista. Tällä tavalla pääsee
myös vakioaikaisesti isännän ja tietyn vasallin structiin käsiksi.

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

    