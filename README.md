# Harry Potter and the Quidditch cup

**Irányítás:**
- Esc - kilépés
- Space - új játék indítása
- I - inverz mód bekapcsolása (ez az alap játék indításakor)
- N - normál mód bekapcsolása (itt a felfelé nyillal megy fel a karakter)

  
- Nyilak - mozgatás (balra, jobbra, fel, le)
- W - karakter előre mozgatása
- S - karakter hátra mozgatása

(Alap esetben nyomva kell tartani a W-t és az S-t, ahoz, hogy előre-hátra menjen, és ha nincs lenyomva
semmi akkor elkezd lassulni. Ezt úgy kell elképzelni mint egy autos videojáték irányítását. Viszont van lehetőség a megfelelő sorok kikommentelésével egy olyan irányítás beálításának ahol a W-vel plusz
sebességet az S-el pedig minusz sebességet adhatunk, ezáltal csak beállítjuk a mozgást es azzal a
sebességgel megy)

**Játék menete:**

A pálya közepén megjelenik Harry és feladata, hogy elkapja az aranycikeszt, anélkül, hogy neki menjen
a talajnak, vagy elüsse egy gurkó, vagy pedig ütközzön a stadion oldalával, vagy elhagyná a területét. A
gurkók random jelennek meg és haladnak át egy bizonyos időközönként a pályán. Az aranycikesz
random irányba mozog néha gyorsabban néha lassabban. Harry mozoghat előre vagy hátra
tetszőleges sebességgel, de tolatás esetén érdemes vigyázni nehogy neki ütközzünk valaminek. A halál
eseteket külön kezeli a játék, azaz haláltól függően kapunk GameOver felületet.
