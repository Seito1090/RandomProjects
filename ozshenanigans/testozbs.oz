functor
import 
   System
   Application
   Open
   OS
   Property
   Browser
define
    proc {Browse Buf}
        {Browser.browse Buf}
    end
    
    fun {ReadFile FileName}
        Words ReturnVal F in
        F = {New Open.file init(name:FileName flags:[read])}
        {F read(list:Words size:all)}
        ReturnVal = {String.tokens Words &\n}
        {F close}
        ReturnVal
    end
    fun {ReadFileThread FileName}
        Value in 
        thread Value = {ReadFile FileName} end 
        Value
    end 
    fun {ReadFileMultiThread FileNames ThreadList}
        case FileNames of nil then ThreadList
        [] H|T then  {ReadFileMultiThread T {ReadFileThread H}|ThreadList}
        end
    end
    fun {ListFiles L Dir Acc}
        case L of nil then Acc
        [] H|T then {ListFiles T Dir Dir#{String.toAtom H}|Acc}
        end
    end

    {Browse {ReadFileMultiThread ["tweets/part_1.txt" "tweets/part_2.txt"] nil}}
    %FileNames = {ListFiles {OS.getDir 'tweets'} 'tweets/' nil}
    %Data = {ReadFileMultiThread FileNames nil}
    %{Browse Data}
end
