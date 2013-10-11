ifj2013
=======

Interpret of php like skripts

Jak rozjet git pod linuxem:<br>
nainstalujete git<br>
nastavte si ssh-key na githubu, navod najdete na googlu<br>
vyvorite si slozku nekde: $ mkdir ifj_project $ cd ifj_project<br>
vytvorite repozitar: $ git init<br>
otevrete soubor config: $ vim .git/config
pridate tam:<br>
[remote "origin"]<br>
url = git@github.com:BigTony/ifj2013.git<br>
fetch = +refs/heads/hvezdicka:refs/remotes/origin/hvezdicka<br>
nevim proc tu ty hvezdicky tak mrdaji :D * <br>

pak by melo stacit uz jen dat $ git pull<br>
a melo by to vse stahnout<br>
kdyz udelate nejaky zmeny tak date $ git commit -am "popis co ste udelali, nejakej kratkej"<br>
a pak $ git push origin<br>

        
	


