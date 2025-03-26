# Maintainer: trianta <gregcraw2001@gmail.com>

pkgname=wrench-git
pkgver=0.0.1
pkgrel=1
pkgdesc="A simple application to handle all of your Linux configuration needs."
arch=(x86_64 aarch64)
url="https://github.com/Trimutex/wrench"
license=('MIT License')
depends=(
  qt6-wayland
)
makedepends=(
  cmake
  git
)
provides=("wrench=${pkgver%%.r*}")
conflicts=(wrench)
source=(
  "git+https://github.com/Trimutex/wrench.git"
)
b2sums=('SKIP')

prepare() {
  cd wrench
}

pkgver() {
	cd "$srcdir/wrench"
  git describe --long --tags --abbrev=8 --exclude='*[a-zA-Z][a-zA-Z]*' \
    | sed -E 's/^[^0-9]*//;s/([^-]*-g)/r\1/;s/-/./g'
}

build() {
  cd "$srcdir/wrench"

  cmake --no-warn-unused-cli -DCMAKE_INSTALL_PREFIX=/usr -S . -B ./build
  cmake --build ./build --target all
}

package() {
  cd "$srcdir/wrench"

  DESTDIR="${pkgdir}" cmake --install build

  # licenses
  install -Dm0644 -t "$pkgdir/usr/share/licenses/${pkgname}" LICENSE
}
