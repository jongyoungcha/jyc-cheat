((nil . ((cmake-ide--build-dir-var . "./")
	 (projectile-project-name . "jyc_cheat_server")
	 (projectile-project-root . "~/projects/jyc-cheat/jyc-cheat-server/")
	 (eval . (defun jyc-sql-compile()
		   (interactive)
		   (shell-command (format "python %s/build.py --debug --cmd" projectile-project-root))
		   (switch-to-buffer-other-window (other-buffer (current-buffer) 1))
		   ))
	 (eval . (defun jyc-sql-compile-exec()
		   (interactive)
		   (exec-shell-command-with-buffer (format "python %s/build.py --debug --cmd --exe" projectile-project-root) (format "%s-%s" "*compilation*" projectile-project-name))
		   (compilation-minor-mode t)
		   (switch-to-buffer-other-window (other-buffer (current-buffer) 1))
		   ))
	 (eval . (local-set-key (kbd "<f5>") 'jyc-sql-compile))
	 (eval . (local-set-key (kbd "<f6>") 'jyc-sql-compile-exec))
	 )))
