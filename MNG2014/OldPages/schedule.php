<?php
require("include/header.php");
?>
<style type="text/css">
.datagrid table {
    border-collapse: collapse;
    text-align: left;
    width: 100%;
    }
.datagrid {
    font: normal 12px/150% Arial, Helvetica, sans-serif;
    background: #48423d;
    overflow: hidden;
    border: 1px solid #4E2F2F;
    -webkit-border-radius: 3px;
    -moz-border-radius: 3px;
    border-radius: 3px;
    }
.datagrid table td, .datagrid table th {
    padding: 3px 10px;
    }
.datagrid table thead th {
    background:-webkit-gradient( linear, left top, left bottom, color-stop(0.05, #4E2F2F), color-stop(1, #4E2F2F) );
    background:-moz-linear-gradient( center top, #4E2F2F 5%, #4E2F2F 100% );
    filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#4E2F2F', endColorstr='#4E2F2F');
    background-color:#4E2F2F;
    color:#FFFFFF;
    font-size: 15px;
    font-weight: bold;
    border-left: 1px solid #EEE8AA;
    }
.datagrid table thead th:first-child {
    border: none;
    }
.datagrid table tbody td {
    color: antiquewhite;
    border-left: 1px solid #4E2F2F;
    font-size: 12px;
    font-weight: normal;
    }
.datagrid table tbody .alt td {
    background: #645b54;
    color: antiquewhite;
    }
.datagrid table tbody td:first-child {
    border-left: none;
    }
.datagrid table tbody tr:last-child td {
    border-bottom: none;
    }
.datagrid table tfoot td div {
    border-top: 1px solid #4E2F2F;
    background: #4E2F2F;
    }
.datagrid table tfoot td {
    padding: 0;
    font-size: 12px;
    }
.datagrid table tfoot td div {
    padding: 2px;
    }
</style>
<div id="wrapper">
<?php
require("include/menu.php");
?>

<div id="contentwrapper">


<div id="content">

<center>
<!--<img src="images/barrett-group-2009-06-web.jpg">-->
</center>


<h2>Schedule:</h2>

<!--

<div class="datagrid"><table>
<col width="25%">
<col width="75%">
<thead>
    <tr>
        <th colspan="2">Friday, Nov 15</th>
    </tr>
</thead>
<tfoot>
    <tr>
        <td colspan="2">
            <div id="no-paging">&nbsp;</div>
    </tr>
</tfoot>
<tbody>
    <tr class="alt">
        <td align="right">9:00 a.m.</td><td><b>Spatiotemporal Patterns in Social Networks</b><br> Nibir Bora, Vladimir Zaytsev, Yu-Han Chang and Rajiv Maheswaran</td>
    </tr>
    <tr>
        <td align="right">9:30 a.m.</td><td><b>A Study of Sourceforge Users and User Network</b><br> Liguo Yu and S. Ramaswamy</td>
    </tr>
    <tr class="alt">
        <td align="right">10:00 a.m.</td><td><b>Mining for Spatially-Near Communities in Geo-Located Social Networks</b><br>Joseph Hannigan, Guillermo Hernandez, Richard M. Medina, Patrick Roos, and Paulo Shakarian</td>
    </tr>
    <tr>
        <td colspan="2" align="center">10:30 a.m. - 11:00 a.m.: Coffee Break</td>
    </tr>
    <tr class="alt">
        <td align="right">11:00 a.m.</td><td><b>Student presentation</b><br> Asaf Beasley</td>
    </tr>
    <tr>
        <td align="right">11:15 a.m.</td><td><b>Student presentation</b><br> Gabriel P&eacute;rez Guzman</td>
    </tr>
    <tr class="alt">
        <td align="right">11:30 a.m.</td><td><b>Student presentation</b><br> Chao Zhang</td>
    </tr>
    <tr>
        <td align="right">11:45 a.m.</td><td><b>Student presentation</b><br> Jun-Ming Xu</td>
    </tr>
    <tr class="alt">
        <td align="right">12:00 p.m.</td><td><b>Student presentation</b><br> Jared Sylvester</td>
    </tr>
    <tr>
        <td align="right">12:15 p.m.</td><td><b>Student presentation</b><br> Jos&eacute; Jim&eacute;nez</td>
    </tr>
    <tr class="alt">
        <td colspan="2" align="center">12:30 p.m. - 2:00 p.m.: Lunch</td>
    </tr>
    <tr>
        <td align="right">2:00 p.m.</td><td><b>A Dynamic Model of Adolescent Smoking: The Role of Peer Networks and Parental Influences</b><br> Cynthia Lakon</td>
    </tr>
    <tr class="alt">
        <td align="right">2:30 p.m.</td><td><b>Prescriptive Analytics for Urban Resilience</b><br> Samarth Swarup</td>
    </tr>
    <tr>
        <td align="right">3:00 p.m.</td><td><b>Modeling Crime Diffusion and Crime Suppression on Transportation Networks: An Initial Report</b><br> Chao Zhang, Albert Xin Jiang, Martin Short, P. Jeffrey Brantingham and Milind Tambe</td>
    </tr>
    <tr class="alt">
        <td colspan="2" align="center">3:30 p.m. - 4:00 p.m.: Coffee Break</td>
    </tr>
    <tr>
        <td align="right">4:00 p.m.</td><td><i>Invited talk</i>: <b>Trust, Influence and Urgent Diffusion in Social Media</b><br>William Rand</td>
    </tr>
    <tr class="alt">
        <td colspan="2" align="center">5:30 p.m. - 6:00 p.m.: Break</td>
    </tr>
    <tr>
        <td colspan="2" align="center">6:00 p.m. - 7:00 p.m.: Reception</td>
    </tr>
</tbody>
</table>
</div>

<div class="datagrid"><table>
<col width="25%">
<col width="75%">
<thead>
    <tr>
        <th colspan="2">Saturday, Nov 16</th>
    </tr>
</thead>
<tfoot>
    <tr>
        <td colspan="2">
            <div id="no-paging">&nbsp;</div>
    </tr>
</tfoot>
<tbody>
    <tr class="alt">
        <td align="right">9:00 a.m.</td><td><i>Invited talk</i>: <b>Entity-Based Data Science</b><br>Lise Getoor</td>
    </tr>
    <tr>
        <td align="right">10:00 a.m.</td><td><b>Associative Patterns of Web-Browsing Behavior</b><br> Myriam Abramson and Shantanu Gore</td>
    </tr>
    <tr class="alt">
        <td colspan="2" align="center">10:30 a.m. - 11:00 a.m.: Coffee Break</td>
    </tr>
    <tr>
        <td align="right">11:00 a.m.</td><td><b>Title TBA</b><br> Milind Tambe</td>
    </tr>
    <tr class="alt">
        <td align="right">11:30 a.m.</td><td><b>Influence Study on Hyper-Graphs</b><br> Dimitrios Vogiatzis</td>
    </tr>
    <tr>
        <td align="right">12:00 p.m.</td><td><b>The Co-evolution of Communication Networks and Drinking Behaviors</b><br>Cheng Wang, David Hachen and Omar Lizardo</td>
    </tr>
    <tr class="alt">
        <td colspan="2" align="center">12:30 p.m. - 2:30 p.m.: Lunch</td>
    </tr>
    <tr>
        <td align="right">2:30 p.m.</td><td><b>Informatics and Analytic Tools to Study Real-World Problems at the Intersection of Public Health and Social Sciences</b><br> Madhav Marathe</td>
    </tr>
    <tr class="alt">
        <td align="right">3:00 p.m.</td><td><b>Agenda Setting and Influence Maximization</b><br> Kiran Lakkaraju</td>
    </tr>
    <tr>
        <td colspan="2" align="center">3:30 p.m. - 4:00 p.m.: Coffee Break</td>
    </tr>
    <tr class="alt">
        <td align="right">4:00 p.m.</td><td><i>Invited talk</i>: <b>Data Tales from the World of Digital Advertising</b><br>Claudia Perlich</td>
    </tr>
    <tr>
        <td colspan="2" align="center">5:30 p.m. - 6:00 p.m.: Break</td>
    </tr>
    <tr class="alt">
        <td colspan="2" align="center">6:00 p.m. - 7:30 p.m.: Plenary Session</td>
    </tr>
    <tr>
        <td colspan="2" align="center">Symposium ends</td>
    </tr>
</tbody>
</table>
</div>

-->

<br><br>
<!--<a href="status.php" title="Project Status">Project Status</a>-->

</div> <!-- content -->
</div> <!-- contentwrapper -->
</div> <!-- wrapper -->

<?php
include("/include/footer.php");
?>
