declare
P S
proc {Print}
   {Show S}
end
{NewPort S P}
{Send P foo}
{Send P bar}
{Print}
{Send P hello}
{Send P test}
{Send P fit}