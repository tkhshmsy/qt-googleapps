%modules = ( # path to module name map
    "GoogleApps" => "$basedir/src/lib/googleapps",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
    "googleoauth2.h" => "GoogleOAuth2",
    "abstractgoogleapps.h" => "AbstractGoogleApps",
    "googledrive.h" => "GoogleDrive",
    "googlecalendar.h" => "GoogleCalendar"
);
%dependencies = (
    "qtbase" => "",
    "qtnetwork" => "",
    "qtnetworkauth" => "",
);

