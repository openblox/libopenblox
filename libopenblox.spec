Name:           libopenblox
Version:        0.1.1
Release:        1%{?dist}
Summary:        The core of the OpenBlox game engine.

License:        LGPLv3+
URL:            https://git.openblox.org/libopenblox.git/
Source0:        https://openblox.org/~johnmh/libopenblox/libopenblox-master.tar.gz

BuildRequires:  qt5-qtbase-devel SDL2-devel libcurl-devel openal-soft-devel freealut-devel ogre-devel lua-devel libraknet-devel
Requires:       qt5-qtbase SDL2 libcurl boost-system openal-soft freealut ogre lua libraknet

%description
The core of the OpenBlox game engine, a library which handles the core logic of the engine.
This library is used by the OpenBlox client `openblox` as well as the OpenBlox server,
`openblox-server`.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains header files for
developing applications that use %{name}.

%prep
%setup -q -n libopenblox-master

%build
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/*
%{_mandir}/man3/*

%files devel
%{_includedir}/*

%changelog
* Sat Feb 13 2016 - 0.1.1
- Initial packaging of libopenblox.
