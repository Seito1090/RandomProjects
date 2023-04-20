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
        Words F in
        F = {New Open.file init(name:FileName flags:[read])}
        {F read(list:Words size:all)}
        {F close}
        Words
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
    %------------------------------------------------------------------------------------------------------------------------------------------        
    fun {HashWorker FileName NGram}
        Quicktable Data Token Hash Name in
        thread  
            Name = FileName.1
            Data = {ReadFile {VirtualString.toAtom "tweets/"#Name}}
            Token = {StringParsing.tokenisingFilter Data}
            Hash = {StringParsing.textTokendToNGramnHashTable Token NGram}
        end
        Hash
    end 

    fun {FuseWorker Hash1 Hash2}
        Y in 
        thread 
            Y = {StringParsing.fuseNGramHashTables Hash1 Hash2}
        end
        Y
    end

    fun {SubProducer Dir FusedHash NGram } %Resposible for getting the fused hash table out 
        Hash1 Hash2 in
        case Dir of nil then FusedHash
        [] H|T then 
            Hash1 = {HashWorker H NGram}
            Hash2 = {SubProducer T FusedHash NGram }
            FusedHash = {FuseWorker Hash1 Hash2}
        end
        FusedHash
    end 

    fun {ReadData Dir NGram} 
        Z Quicktable FusedHash H1 in 
        thread %this thread goes on and creates new threads that worl on the files in the directory  
            FusedHash = {SubProducer Dir nil NGram}
            Z = {StringParsing.countHashesFromTable FusedHash}
            Quicktable = {StringParsing.tableToQuicktable Z}
        end
        Quicktable
    end
    {Browse {ReadData {ListFiles {OS.getDir 'tweets'} 'tweets/' nil} 2}}

    %{Browse {ReadFileMultiThread ["tweets/part_1.txt" "tweets/part_2.txt"] nil}}
    %FileNames = {ListFiles {OS.getDir 'tweets'} 'tweets/' nil}
    %Data = {ReadFileMultiThread FileNames nil}
    %{Browse Data}
end

%%% Example : 
thread 
    local NGram FILE1 TempFile1 FILE2 TempFile2 Data1 Data2 Token1 Token2 Hash1 Hash2 Y Z Quicktable in 
        NGram = 2
        FILE1 = {OS.getDir 'tweets'}
        FILE2 = FILE1.2
        TempFile1 = FILE1.1
        TempFile2 = FILE2.1
        Data1 = {ReadFile {VirtualString.toAtom "tweets/"#TempFile1}}
        Data2 = {ReadFile {VirtualString.toAtom "tweets/"#TempFile2}}
        Token1 = {StringParsing.tokenisingFilter Data1}
        Token2 = {StringParsing.tokenisingFilter Data2}
        Hash1 = {StringParsing.textTokendToNGramnHashTable Token1 NGram}
        Hash2 = {StringParsing.textTokendToNGramnHashTable Token2 NGram}

        Y = {StringParsing.fuseNGramHashTables Hash1 Hash2}
        Z = {StringParsing.countHashesFromTable Y}
        Quicktable = {StringParsing.tableToQuicktable Z}

        %Broswers here check later on
    end
end