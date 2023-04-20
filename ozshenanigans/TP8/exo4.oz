declare 
fun {OddFilter Lst Acc}
   case Lst of nil then Acc
   [] H|T then if H mod 2 == 0 then {OddFilter T Acc} else {OddFilter T H|Acc}end
   end
end
MaListe = 1|2|3|4|5|6|7|8|9|10|nil
{Browse {OddFilter MaListe nil}}

declare
fun {ServeBeer} Beer in
   %Fonction qui renvoie une bière 
    if %qq chose 
    then Beer = true
    else Beer = false
    end
end
fun {SmellTrappist Beer}
   %Fonctio qui renvoie true ou false dépendemment si la bière est trappiste ou pas 
   Beer
end
fun {Barman N} Stream in
   %Utilise la fonction delay pour faire l'intervale de temps
   %Gen un stream de N bierres avec un intervalle de temps de 3 secondes 
    case N of 1 then {ServeBeer}
    else {Delay 3000} {ServeBeer} {Barman N-1}
    end
end
%Compter le nombre de fois qu'il y a eu true -> donner le nombre de true et false comme output