$this->IncludeElement("sortoption");

$sortoption = new CSortOptionElement(
	$this->GetWebsite(),
	"default");

$strSortOption = $this->GetModuleVar("sort_option");
$strSortOrder = $this->GetModuleVar("sort_order");

$arrayData = array(
	array(
		$sortoption->RenderSortOption(
			$this->GetModuleVarName("sort_option"),
			"1",
			$this->GetModuleVarName("sort_order"),
			($strSortOption == "1")?($strSortOrder):(""),
			"?TID_TOOLTIP_SORTASC?",
			"?TID_TOOLTIP_SORTDESC?") . "Fruit"
		,
		$sortoption->RenderSortOption(
			$this->GetModuleVarName("sort_option"),
			"2",
			$this->GetModuleVarName("sort_order"),
			($strSortOption == "2")?($strSortOrder):(""),
			"?TID_TOOLTIP_SORTASC?",
			"?TID_TOOLTIP_SORTDESC?") . "Amount"
		)
	,
	array("Apple", "1")
	);

$table = new CTableElement($this->GetWebsite(),"default");
$strOutput = $table->RenderTable($arrayData,true,false);