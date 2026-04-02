declare-option str-list kakodemon_masters

hook -group kakodemon global ClientCreate .* %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit
        [ -z "$kak_client_env_KAKOD_CLIENT_ID" ] && exit

        export KAKOD_ID=$kak_client_env_KAKOD_ID

        kakod -p rename-session $kak_session
        kakod -p rename-client $kak_client_env_KAKOD_CLIENT_ID $kak_client
    }
}

hook -group kakodemon global ClientRenamed .* %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit
        [ -z "$kak_client_env_KAKOD_CLIENT_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p rename-client $kak_client_env_KAKOD_CLIENT_ID $kak_client
    }
}

hook -group kakodemon global SessionRenamed .* %{
    evaluate-commands -client * %{
        nop %sh{
            [ -z "$kak_client_env_KAKOD_ID" ] && exit

            KAKOD_ID=$kak_client_env_KAKOD_ID \
            kakod -p rename-session $kak_session
        }
     }
 }

define-command kakodemon-new-client -params .. %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p new-client "$@"
    }
}

complete-command kakodemon-new-client command

alias global new kakodemon-new-client

define-command kakodemon-focus -params 1 %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p focus $1
    }
}

complete-command -menu kakodemon-focus client

alias global focus kakodemon-focus

define-command -params 1 kakodemon-layout %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p set-layout $1
    }
}

complete-command -menu kakodemon-layout shell-script-candidates %{ printf 'tall\nwide\nfull\n' }
