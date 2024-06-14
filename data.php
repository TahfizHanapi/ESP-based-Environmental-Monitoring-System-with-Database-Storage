<?php
class dht11 {
    public $link='';

    function __construct($temperature, $humidity, $distance){
        $this->connect();
        $this->storeInDB($temperature, $humidity, $distance);
    }

    function connect(){
        $this->link = mysqli_connect('localhost', 'root', 'Tahfiz_2002') or die('Cannot connect to the DB');
        mysqli_select_db($this->link, 'environmentcensor') or die('Cannot select the DB');
    }

    function storeInDB($temperature, $humidity, $distance){
        // Get current timestamp
        $timestamp = date('Y-m-d H:i:s');

        // Escape values to prevent SQL injection
        $temperature = mysqli_real_escape_string($this->link, $temperature);
        $humidity = mysqli_real_escape_string($this->link, $humidity);
        $distance = mysqli_real_escape_string($this->link, $distance);

        // Insert query with timestamp
        $query = "INSERT INTO environment (temperature, humidity, distance, timestamp) 
                  VALUES ('$temperature', '$humidity', '$distance', '$timestamp')";
        $result = mysqli_query($this->link, $query) or die('Errant query: '.$query);
    }
}

if(isset($_GET['temperature']) && isset($_GET['humidity']) && isset($_GET['distance'])){
    $dht11 = new dht11($_GET['temperature'], $_GET['humidity'], $_GET['distance']);
}
?>
