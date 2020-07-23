
open System

let makeSymbol (x: char) =
    match x with
    | '0' | 'O' | 'o' ->
        0
    | '1' | 'I' | 'i' | 'L' | 'l' ->
        1
    | '2' -> 2
    | '3' -> 3
    | '4' -> 4
    | '5' -> 5
    | '6' -> 6
    | '7' -> 7
    | '8' -> 8
    | '9' -> 9
    | 'A' | 'a' -> 10
    | 'B' | 'b' -> 11
    | 'C' | 'c' -> 12
    | 'D' | 'd' -> 13
    | 'E' | 'e' -> 14
    | 'F' | 'f' -> 15
    | 'G' | 'g' -> 16
    | 'H' | 'h' -> 17
    | 'J' | 'j' -> 18
    | 'K' | 'k' -> 19
    | 'M' | 'm' -> 20
    | 'N' | 'n' -> 21
    | 'P' | 'p' -> 22
    | 'Q' | 'q' -> 23
    | 'R' | 'r' -> 24
    | 'S' | 's' -> 25
    | 'T' | 't' -> 26
    | 'V' | 'v' -> 27
    | 'W' | 'w' -> 28
    | 'X' | 'x' -> 29
    | 'Y' | 'y' -> 30
    | 'Z' | 'z' -> 31
    | _ -> 0xFF

let makeItems () =
    seq {
        for x in 0..255 do
            yield (sprintf "0x%02Xu" (makeSymbol (char x)))
    }

let lines =
    makeItems ()
    |> Seq.chunkBySize 8
    |> Seq.map (fun x -> String.Join (", ", x))

printfn "%s" (String.Join (",\n", lines))