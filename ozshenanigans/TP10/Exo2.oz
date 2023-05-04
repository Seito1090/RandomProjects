declare A B N S Res1 Res2 Res3 Res4 Res5 Res6 
fun {LaunchServer} 
    Portp S
    proc {Server L}
        case L of nil then skip
        [] add(X Y R)|T then 
            R = X+Y
            {Server T}
        [] pow(X N R)|T then 
            R = {Pow X N}
            {Server T}
        [] 'div'(X Y R)|T then 
            R = X/Y
            {Server T}
        else {Browse 'je ne comprends po'}
        end
    end
in
    Portp = {NewPort S}
    thread {Server S} end
    Portp
end


S = {LaunchServer}
{Send S add(321 345 Res1)}
{Browse Res1}

{Send S pow(2 N Res2)}
N = 8 
{Browse Res2}

{Send S add(A B Res3)}
{Send S add(10 20 Res4)}
{Send S foo}
{Browse Res4}
A = 3 
B = 0-A 
{Send S 'div'(90 Res3 Res5)}
{Send S 'div'(90 Res4 Res6)}
{Browse Res3}
{Browse Res5}
{Browse Res6}   
