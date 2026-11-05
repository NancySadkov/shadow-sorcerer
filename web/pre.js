/* Emscripten pre-run: put the game data on a persistent IDBFS volume.
 *
 * Assets are preloaded read-only under /assets/game.  The writable /game
 * directory is an IndexedDB mount; on first run the assets are copied into
 * it, and on later runs only missing files are added, so save files written
 * by the game survive a reload.
 */
(function () {
  function install_case_insensitive_open() {
    if (FS.caseInsensitiveInstalled)
      return;
    FS.caseInsensitiveInstalled = true;
    var orig_open = FS.open;
    FS.open = function (path, flags, mode) {
      try {
        return orig_open.call(FS, path, flags, mode);
      } catch (e) {
        if (!e || e.name !== 'ErrnoError' || e.errno !== 44)
          throw e;
        var p = String(path);
        if (p.charAt(0) !== '/')
          p = FS.cwd() + '/' + p;
        var parts = p.split('/');
        var base = parts.pop();
        var dir = parts.join('/') || '/';
        var entries;
        try {
          entries = FS.readdir(dir);
        } catch (_) {
          throw e;
        }
        var match = null;
        for (var i = 0; i < entries.length; i++) {
          if (entries[i].toLowerCase() === base.toLowerCase()) {
            match = entries[i];
            break;
          }
        }
        if (!match)
          throw e;
        return orig_open.call(FS, dir + '/' + match, flags, mode);
      }
    };
  }

  function copy_dir(src, dst) {
    if (!FS.analyzePath(dst).exists)
      FS.mkdir(dst);
    var entries = FS.readdir(src);
    for (var i = 0; i < entries.length; i++) {
      var name = entries[i];
      if (name === '.' || name === '..')
        continue;
      var s = src + '/' + name;
      var d = dst + '/' + name;
      var st = FS.stat(s);
      if (FS.isDir(st.mode)) {
        copy_dir(s, d);
      } else if (!FS.analyzePath(d).exists) {
        FS.writeFile(d, FS.readFile(s));
      }
    }
  }

  function assets_ready() {
    try {
      if (!FS.analyzePath('/assets/game').exists)
        return false;
      var entries = FS.readdir('/assets/game');
      for (var i = 0; i < entries.length; i++) {
        if (entries[i] !== '.' && entries[i] !== '..')
          return true;
      }
    } catch (e) {
    }
    return false;
  }

  Module.preRun = Module.preRun || [];
  Module.preRun.push(function () {
    Module.addRunDependency('idbfs');
    install_case_insensitive_open();

    function begin() {
      if (!assets_ready()) {
        setTimeout(begin, 20);
        return;
      }
      try {
        if (!FS.analyzePath('/game').exists)
          FS.mkdir('/game');
        FS.mount(IDBFS, {}, '/game');
        FS.syncfs(true, function (err) {
          if (err)
            console.error('IDBFS load failed', err);
          try {
            copy_dir('/assets/game', '/game');
          } catch (e) {
            console.error('game asset copy failed', e);
          }
          FS.syncfs(false, function (err2) {
            if (err2)
              console.error('IDBFS save failed', err2);
            Module.removeRunDependency('idbfs');
          });
        });
      } catch (e) {
        console.error('IDBFS setup failed', e);
        Module.removeRunDependency('idbfs');
      }
    }

    begin();
  });
})();
