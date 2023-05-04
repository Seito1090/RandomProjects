declare 
fun {StudentRMI} S in 
    thread 
        for ask(howmany:Beers) in S do
            Beers={OS.rand} mod 24
        end 
    end
    {NewPort S}
end
fun {StudentCallBack} S in 
    thread
        for ask(howmany:P) in S do 
            {Send P {OS.rand} mod 24}
        end
    end
    {NewPort S}
end
fun {CreateUniversity Size}
    fun {CreateLoop I}
        if I =< Size then %soit StudentRmi soit StudentCallBack
            {StudentRMI}|{CreateLoop I+1}
        else nil end
    end
in
    {CreateLoop 1}
end