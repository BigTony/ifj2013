ifj2013
=======

Interpret of php like skripts

Jak rozjet git pod linuxem:<br>
<t>nainstalujete git<br>
	vyvorite si slozku nekde: $ mkdir ifj_project $ cd ifj_project<br>
	vytvorite repozitar: $ git init<br>
	otevrete soubor config: $ vim .git/config
	pridate tam:<br>
	[remote "origin"]<br>
        url = git@github.com:BigTony/ifj2013.git<br>
        fetch = +refs/heads/*:refs/remotes/origin/*<br>
	


