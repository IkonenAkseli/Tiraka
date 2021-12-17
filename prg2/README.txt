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