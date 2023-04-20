declare
fun {ProduceInt N} Stream in
   case N of 1 then 1
   else Stream = N|{ProduceInt N-1}
   end
end
fun {Sum Str}
   case Str of 1 then 1
   [] H|T then H+{Sum T}
   else Str
   end
end
Xs S
thread Xs = {ProduceInt 666666} end
thread S = {Sum Xs} end
{Browse S}
declare Xs S
Xs = {ProduceInt 666666}
S = {Sum Xs}
{Browse S}
