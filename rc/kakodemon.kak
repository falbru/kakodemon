declare-option str-list kakodemon_masters

hook -group kakodemon global ClientCreate .* %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit
        [ -z "$kak_client_env_KAKOD_CLIENT_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
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

define-command kakodemon-new-client %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        ~/code/github.com/falbru/kakodemon/build/kakod -p new-client
    }
}

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

complete-command -menu kakodemon-layout shell-script-candidates %{ printf 'tall\nwide\n' }
