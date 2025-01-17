#	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
#
#	Copyright (C) 2015-2022 Barcelona Supercomputing Center (BSC)


AC_DEFUN([AC_CHECK_COMPILER_FLAG],
	[

		ac_save_[]_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS"
		AC_MSG_CHECKING([if $[]_AC_CC[] $[]_AC_LANG_PREFIX[]FLAGS supports the $1 flag])
		_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS $1"
		AC_LINK_IFELSE(
			[AC_LANG_PROGRAM([[]], [[]])],
			[
				AC_MSG_RESULT([yes])
			], [
				AC_MSG_RESULT([no])
				_AC_LANG_PREFIX[]FLAGS="$ac_save_[]_AC_LANG_PREFIX[]FLAGS"

				if test x"${USING_MERCURIUM}" = x"yes" ; then
					AC_MSG_CHECKING([if $[]_AC_CC[] $[]_AC_LANG_PREFIX[]FLAGS supports the $1 flag through the preprocessor])
					_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS --Wp,$1"
					AC_LINK_IFELSE(
						[AC_LANG_PROGRAM([[]], [[]])],
						[
							AC_MSG_RESULT([yes])
							# Save the flag
							ac_save_[]_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS"
						], [
							AC_MSG_RESULT([no])
							_AC_LANG_PREFIX[]FLAGS="$ac_save_[]_AC_LANG_PREFIX[]FLAGS"
						]
					)

					AC_MSG_CHECKING([if $[]_AC_CC[] $[]_AC_LANG_PREFIX[]FLAGS supports the $1 flag through the native compiler])
					_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS --Wn,$1"
					AC_LINK_IFELSE(
						[AC_LANG_PROGRAM([[]], [[]])],
						[
							AC_MSG_RESULT([yes])
							# Save the flag
							ac_save_[]_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS"
						], [
							AC_MSG_RESULT([no])
							_AC_LANG_PREFIX[]FLAGS="$ac_save_[]_AC_LANG_PREFIX[]FLAGS"
						]
					)

					AC_MSG_CHECKING([if $[]_AC_CC[] $[]_AC_LANG_PREFIX[]FLAGS supports the $1 flag through the linker])
					_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS --Wl,$1"
					AC_LINK_IFELSE(
						[AC_LANG_PROGRAM([[]], [[]])],
						[
							AC_MSG_RESULT([yes])
						], [
							AC_MSG_RESULT([no])
							_AC_LANG_PREFIX[]FLAGS="$ac_save_[]_AC_LANG_PREFIX[]FLAGS"
						]
					)
				fi
			]
		)

	]
)


AC_DEFUN([AC_CHECK_COMPILER_FLAGS],
	[
		for check_flag in $1 ; do
			AC_CHECK_COMPILER_FLAG([$check_flag])
		done
	]
)


AC_DEFUN([AC_CHECK_FIRST_COMPILER_FLAG],
	[
		ac_save2_[]_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS"
		for flag in $1 ; do
			AC_CHECK_COMPILER_FLAG([$flag])
			if test x"$ac_save2_[]_AC_LANG_PREFIX[]FLAGS" != x"$[]_AC_LANG_PREFIX[]FLAGS" ; then
				break;
			fi
		done
	]
)


dnl AC_CHECK_EXTRACT_FIRST_COMPILER_FLAG(VARIABLE-NAME, [list of flags])
AC_DEFUN([AC_CHECK_EXTRACT_FIRST_COMPILER_FLAG],
	[
		ac_save2_[]_AC_LANG_PREFIX[]FLAGS="$[]_AC_LANG_PREFIX[]FLAGS"
		for flag in $2 ; do
			AC_CHECK_COMPILER_FLAG([$flag])
			if test x"$ac_save2_[]_AC_LANG_PREFIX[]FLAGS" != x"$[]_AC_LANG_PREFIX[]FLAGS" ; then
				$1[]="$flag"
				break;
			fi
		done
		[]_AC_LANG_PREFIX[]FLAGS="$ac_save2_[]_AC_LANG_PREFIX[]FLAGS"
		AC_SUBST($1)
	]
)


# This should be called before AC_PROG_CXX
AC_DEFUN([SSS_PREPARE_COMPILER_FLAGS],
	[
		AC_ARG_VAR(OPT_CXXFLAGS, [C++ compiler flags for optimized versions. Notice that setting CXXFLAGS may override these flags])
		AC_ARG_VAR(DBG_CXXFLAGS, [C++ compiler flags for debugging versions. Notice that setting CXXFLAGS may override these flags])

		user_CXXFLAGS="${CXXFLAGS}"

		# Do not let autoconf set up its own set of configure flags
		CXXFLAGS=" "
	]
)


# This should be called after the value of CXXFLAGS has settled
AC_DEFUN([SSS_FIXUP_COMPILER_FLAGS],
	[
		AC_LANG_PUSH(C++)

		AC_CHECK_COMPILER_FLAGS([-Wall -Wextra -Wdisabled-optimization -Wshadow -fvisibility=hidden])

		autoconf_calculated_cxxflags="${CXXFLAGS}"

		# Fill in DBG_CXXFLAGS
		if test x"${DBG_CXXFLAGS}" != x"" ; then
			DBG_CXXFLAGS="${autoconf_calculated_cxxflags} ${DBG_CXXFLAGS} ${user_CXXFLAGS}"
		else
			AC_CHECK_COMPILER_FLAG([-O0])
			AC_CHECK_FIRST_COMPILER_FLAG([-g3 -g2 -g])
			AC_CHECK_FIRST_COMPILER_FLAG([-fstack-security-check -fstack-protector-all])
			DBG_CXXFLAGS="${CXXFLAGS} ${user_CXXFLAGS}"
		fi

		# Fill in OPT_CXXFLAGS
		CXXFLAGS="${autoconf_calculated_cxxflags}"
		if test x"${OPT_CXXFLAGS}" != x"" ; then
			OPT_CXXFLAGS="${autoconf_calculated_cxxflags} ${OPT_CXXFLAGS} ${user_CXXFLAGS}"
		else
			AC_CHECK_FIRST_COMPILER_FLAG([-O3 -O2 -O])
			OPT_CLANG_CXXFLAGS="${CXXFLAGS} ${user_CXXFLAGS}"
			AC_CHECK_COMPILER_FLAG([-flto])
			OPT_CXXFLAGS="${CXXFLAGS} ${user_CXXFLAGS}"
		fi

		CXXFLAGS="${autoconf_calculated_cxxflags}"

		AC_SUBST(DBG_CXXFLAGS)
		AC_SUBST(OPT_CXXFLAGS)
		AC_SUBST(OPT_CLANG_CXXFLAGS)

		AC_CHECK_EXTRACT_FIRST_COMPILER_FLAG([FALIGNED_NEW_FLAG], [-faligned-new])

		AC_CHECK_EXTRACT_FIRST_COMPILER_FLAG([MCMODEL_FLAGS], [-mcmodel=large -mcmodel=medium])

		AC_LANG_POP(C++)
	]
)

