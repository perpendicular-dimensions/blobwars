Summary: Mission and Objective based 2D Platform Game
Name: %{name}
Version: %{version}
Release: %{release}
Source: %{name}-%{version}-%{release}.tar.gz
Vendor: Parallel Realities
Packager: Stephen Sweeney
URL: http://www.parallelrealities.co.uk/blobWars.php
Group: Games/Arcade
License: GPL
%description
Blob Wars : Metal Blob Solid. This is Episode I of the Blob Wars Saga.
You must undertake the role of fearless Blob solider, Bob, as he infiltrates
various enemy installations and hideouts in an attempt to rescue as many
MIAs as possible.

%prep
%setup -q

%build
make VERSION=%{version} RELEASE=%{release}

%install
make install

%clean

%post


%files
%dir /usr/games
%dir /usr/share/games/blobwars
%dir /usr/share/doc/%{name}
%dir /usr/share/applnk
/usr/games/%{name}
/usr/share/games/blobwars/%{name}.pak
/usr/share/doc/%{name}/*
/usr/share/applications/blobwars.desktop
/usr/share/icons/hicolor/16x16/apps/blobwars.png
/usr/share/icons/hicolor/32x32/apps/blobwars.png
/usr/share/icons/hicolor/64x64/apps/blobwars.png
/usr/share/locale/ca/LC_MESSAGES/blobwars.mo
/usr/share/locale/da/LC_MESSAGES/blobwars.mo
/usr/share/locale/de/LC_MESSAGES/blobwars.mo
/usr/share/locale/es/LC_MESSAGES/blobwars.mo
/usr/share/locale/nl/LC_MESSAGES/blobwars.mo
/usr/share/locale/pl/LC_MESSAGES/blobwars.mo
/usr/share/locale/sv/LC_MESSAGES/blobwars.mo
