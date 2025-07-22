program Main;
var a, b: integer;

function Square(x: integer): integer;
begin
    Square := x * x;
end;

begin
    a := 5;
    b := Square(a);
    writeln("Square of ", a, " is ", b);
end.