
                           biblio.module

Author:  Ron Jerome (ron.jerome@nrc.ca)
Released under the GPL


Description:
============
This module extends the node data type with additional fields to manage lists 
of scholarly publications.

It closely follows the EndNote model, thus both importing from and exporting 
to Endote are supported. Other formats could be added if there was sufficient 
demand.

Bibliographic information is displayed in lists with links to detailed 
information on each publication.

The lists can be sorted, filtered and ordered in many different ways.

See liiscience.org for a live example.


Requirements:
=============
Drupal 4.6.x.  This module has not been tested with any other version of Drupal 
at the time of writing.

The domxml php extension must be enabled in order to import or export XML files.


Installation:
=============
Create a directory called biblio in the modules directory, then place all of the
files packaged with this module in that directory.

This module requires additional tables to store information.  These tables can 
be created using following command:

mysql -u {userid} -p {drupaldatabase} < biblio.mysql

You will also have to enable the module on the admin/modules page.


Settings:
=========
A number of settings are available at admin/settings/biblio.  They control how 
the author names are displayed, whether export links are added to pages and the
number of entries per page to display.

Access Control:
===============
Three permissions are controlable on the admin/access page.  I think they are fairly
self evident, they control who can create biblio entries, edit entries and who can
import from file.

Adding/importing records:
=========================
Bibliographic entries can be added to the database in one of two ways, individualy
from the node/add/biblio link, or by importing records from Endnote in their "Tagged"
file format.  Administrators can go to admin/settings/biblio/import and fill in 
the form to upload and import of an Endnote tagged file.


Features:
=========
By default, the /biblio page will list all of the entries in the database sorted
by Year in descending order. If you wish to sort by "Title" or "Type", you may 
do so by clicking on the appropriate links at the top of the page. To reverse 
the sort order, simply click the link a second time.


Filtering Search Results:
=========================
If you wish to filter the results, click on the "Filter" tab at the top of the 
page. To add a filter, click the radio button to the left of the filter type 
you wish to apply, then select the filter criteria from the drop down list 
on the right, then click the filter button.

It is possible to create complex filters by returning to the "Filter" tab and 
adding additional filters. Simply follow the steps outlined above and press 
the "Refine" button.

All filters can be removed by clicking the Clear All Filters link at the top 
of the result page, or on the "Filter" tab they can be removed one at a time 
using the "Undo" button, or you can remove them all using the "Clear All" button

You may also construct URLs which filter. For example, /biblio/year/2005 will 
show all of the entries for 2005. /biblio/year/2005/author/smith will show all 
of entries from 2005 for smith.


Exporting Search Results:
=========================
Assuming this option has been enabled by the administrator, you can export 
search results directly into EndNote. The link at the top of the result page 
will export all of the search results, and the links on individual entries will 
export the information related to that single entry.

Clicking on one of the export links should cause your browser to ask you 
whether you want to Open, or Save To Disk, the file endnote.enw. If you choose 
to open it, Endnote should start and ask you which library you would like 
store the results in. Alternatively, you can save the file to disk and manually 
import it into EndNote.


The information is exported in either EndNote "Tagged" format similar to this...

              %0  Book
              %A  John Smith 
              %D  1959
              %T  The Works of John Smith
              ...
              
Or Endnote 7 XML format which is similar to this...

              <XML>
              	<RECORDS>
              	  <RECORD>
                    <REFERENCE_TYPE>10</REFERENCE_TYPE>
                    <YEAR>1959</YEAR>
              	    <TITLE>The Works of John Smith</TITLE>
              	    <AUTHORS>
                      <AUTHOR>John Smith </AUTHOR>
                    </AUTHORS>
                  </RECORD>
                </RECORDS>
              </XML>
              
              
