$this->IncludeElement('legend');

$legend = new CLegendElement($this->GetWebsite(),'default');
$arrayItems = array();

$arrayItems[] = array(
	'icon' => 'square',
	'label' => 'Blue',
	'color' => 'blue'
	);

$arrayItems[] = array(
	'icon' => 'square',
	'label' => 'Red',
	'color' => 'red'
	);

$arrayItems[] = array(
	'icon' => 'square',
	'label' => 'Green',
	'color' => 'green'
	);

$strOutput = $legend->RenderLegend($arrayItems);