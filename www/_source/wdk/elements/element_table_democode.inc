$table = new CTableElement($this->GetWebsite(),"default");
$arrayTable = array();
for ($nRowIndex = 0; $nRowIndex < 10; $nRowIndex++)
{
	$arrayCols = array();
	for ($nColIndex = 0; $nColIndex < 5; $nColIndex++)
	{
		$arrayCols[$nColIndex] = "Row$nRowIndex Col$nColIndex";
	}
	$arrayTable[$nRowIndex] = $arrayCols;
}
$strOutput = $table->RenderTable($arrayTable, true, true);

$strOutput .= "<br/>"; 

$arrayHeaders = array();
$arrayHeaders[0][0] = true;
$arrayHeaders[1][1] = true;
$arrayHeaders[2][0] = true;
$arrayHeaders[3][1] = true;
$arrayHeaders[4] = array(1,1,1,1,1);
$arrayHeaders[6] = array(0,0,0,0,1);

$arrayCellStyle = array();
$arrayCellStyle[0][4] = "right";
$arrayCellStyle[3] = array("left","center","center","center","right"); // default cell style is left aligned

$strOutput .= $table->RenderTableEx($arrayTable, $arrayHeaders,$arrayCellStyle);