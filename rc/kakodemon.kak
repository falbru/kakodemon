hook -group kakodemon global ClientCreate .* %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p rename-session $kak_session
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

define-command kakodemon-new-client %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        ~/code/github.com/falbru/kakodemon/build/kakod -p new-client
    }
}

alias global new kakodemon-new-client

define-command -params 1 kakodemon-layout %{
    nop %sh{
        [ -z "$kak_client_env_KAKOD_ID" ] && exit

        KAKOD_ID=$kak_client_env_KAKOD_ID \
        kakod -p set-layout $1
    }
}

complete-command -menu kakodemon-layout shell-script-candidates %{ printf 'tall\nwide\n' }
